// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/UPPettuCharacter.h"

#include "BrainComponent.h"
#include "AI/UPPettuAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Weapon/UPPettuWeapon.h"
#include "Components/UPCharacterStatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/HUD.h"
#include "UI/UPHudWidget.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Character/UPPlayerCharacter.h"
#include "Game/UPGameMode.h"
#include "Manager/UPSequenceHandler.h"


AUPPettuCharacter::AUPPettuCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UUPCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	static ConstructorHelpers::FObjectFinder<UUPCharacterStatData> StatDataRef(TEXT("/Game/UniversityProject/GameData/DA_PettuCharacterStat.DA_PettuCharacterStat"));
	StatComponent->SetBaseStat(StatDataRef.Object.Get()->Stat);
	
	bIsStiffen = false;
	bIsStun = false;
	bIsDead = false;
	CurrentPhase = EBossPhase::Phase1;
}

void AUPPettuCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//StatComponent->OnHpZero.AddUObject(this, &AUPPettuCharacter::SetDead);
	//StatComponent->OnStunStackZero.AddUObject(this, &AUPPettuCharacter::SetStun);
	StatComponent->OnStiffen.AddUObject(this, &AUPPettuCharacter::SetStiffen);
	StatComponent->OnHpChanged.AddUObject(this, &AUPPettuCharacter::StunCheck);
	MonsterAIController = Cast<AUPPettuAIController>(GetController());

	// 플레이어의 최대 기울기 설정 (보스 머리 위에서 미끄러지게 함)
	//GetCharacterMovement()->Walkable(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 30.f));

	//GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetSimulatePhysics(false);
	MovementComponent->bEnablePhysicsInteraction = true;
	MovementComponent->bPushForceUsingZOffset = true;
}

void AUPPettuCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = StatComponent->GetBaseStat().WalkSpeed;
	PlayerCharacter = Cast<AUPPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (PlayerCharacter)
	{
		PlayerCharacter->SetPettuCharacter(this);
	}

	/* Set HUD */
	AUPPlayerController* PlayerController = Cast<AUPPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController && PlayerController->GetHUD())
	{
		UUPHudWidget* HudWidget = Cast<UUPHudWidget>(PlayerController->GetHUD());
		SetupHUDWidget(HudWidget);
	}
	
	PlayerController->SetGameMode();
	PlayerController->HudWidgetObject->SetVisibility(ESlateVisibility::Visible);
	PlayerController->HudWidgetObject->SetPettuHudVisible(true);
	PlayerController->HudWidgetObject->SetPlayerHudVisible(true);
}

float AUPPettuCharacter::UPTakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                                      AActor* DamageCauser)
{
	return Super::UPTakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void AUPPettuCharacter::SetDead()
{
	if (CurrentPhase == EBossPhase::Phase1)
	{
		bIsInvincible = true;
		GetMesh()->SetVisibility(false, true);
		
		// BT 중지
		MovementComponent->DisableMovement();
		if (MonsterAIController)
		{
			if (MonsterAIController->BrainComponent)
			{
				MonsterAIController->BrainComponent->StopLogic(TEXT("Dead"));
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("BrainComponent is nullptr"));
			}
			UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(MonsterAIController->BrainComponent);
			if (BTComp)
			{
				BTComp->StopTree(EBTStopMode::Forced);
			}
			MonsterAIController->StopMovement();
		}
		
		// 보스 시퀀서 시작
		// 시퀀서 종료 시, 보스 2페이즈로 변경 및 전투 시작.
		UUPSequenceHandler* SequenceHandler = GetGameInstance()->GetSubsystem<UUPSequenceHandler>();
		if (SequenceHandler != nullptr)
		{
			SequenceHandler->PlaySequence(Boss2PhaseStartSequence);
			SequenceHandler->GetCurrentSequence()->OnFinished.AddDynamic(this, &AUPPettuCharacter::Phase2Start);
		}
	}
	else if (CurrentPhase == EBossPhase::Phase2)
	{
		Super::SetDead();
	}
	CurrentPhase = EBossPhase::Dead;
}

void AUPPettuCharacter::PlayDeadAnimation()
{
	Super::Super::PlayDeadAnimation();
}

void AUPPettuCharacter::Phase2Start()
{
	bIsInvincible = false;

	// 머티리얼 세팅
	UMaterialInterface* OverlayMaterial = Phase2OutLineMaterial;
        
	// 오버레이 머티리얼이 있다면 동적으로 설정
	if (OverlayMaterial)
	{
		DynamicMaterial = UMaterialInstanceDynamic::Create(OverlayMaterial, this);
		GetMesh()->SetOverlayMaterial(DynamicMaterial);
	}

	
	// 스텟 세팅 및 초기화
	check(StatComponent != nullptr);
	check(CharacterInitalizeStatData != nullptr);	
	StatComponent->SetBaseStat(Phase2InitslizeStatData->Stat);

	GetMesh()->SetVisibility(true, true);
	
	// BT 변경 m및 재시작
	if (MonsterAIController)
	{
		if (Phase2BehaviorTree)
		{
			MonsterAIController->RunBehaviorTree(Phase2BehaviorTree);
		}

		UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(MonsterAIController->BrainComponent);
		if (BTComp)
		{
			BTComp->RestartTree();
		}
	}

	// 이동 활성화
	MovementComponent->SetMovementMode(MOVE_Walking);

	// 현재 스테이트 변경
	CurrentPhase = EBossPhase::Phase2;

	// 델리게이트 제거
	UUPSequenceHandler* SequenceHandler = GetGameInstance()->GetSubsystem<UUPSequenceHandler>();
	if (SequenceHandler)
	{
		ULevelSequencePlayer* CurrentSequence = SequenceHandler->GetCurrentSequence();
		if (CurrentSequence)
		{
			CurrentSequence->OnFinished.RemoveDynamic(this, &AUPPettuCharacter::Phase2Start);
		}
	}
}

void AUPPettuCharacter::DeadAnimEnd(UAnimMontage* Montage, bool bInterrupted)
{
	Super::DeadAnimEnd(Montage, bInterrupted);

	// 승리
	AUPGameMode* GameMode = Cast<AUPGameMode>(GetWorld()->GetAuthGameMode());
	check(GameMode != nullptr);
	GameMode->OnGameClear();
	Destroy();

	/* Set HUD */
	AUPPlayerController* PlayerController = Cast<AUPPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController && PlayerController->GetHUD())
	{
		UUPHudWidget* HudWidget = Cast<UUPHudWidget>(PlayerController->GetHUD());
		SetupHUDWidget(HudWidget);
	}
	
	PlayerController->SetGameMode();
	PlayerController->HudWidgetObject->SetPettuHudVisible(false);
	PlayerController->HudWidgetObject->SetVisibility(ESlateVisibility::Hidden);
}

void AUPPettuCharacter::SetStun()
{
	Super::SetStun();
	StatComponent->ApplyStunStack(1);
	if (MonsterAIController)
	{
		UBlackboardComponent* BlackboardComp = MonsterAIController->GetBlackboardComponent();
		if (BlackboardComp)
		{
			BlackboardComp->SetValueAsBool(TEXT("IsStun"), bIsStun);
		}
	}
	GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &AUPPettuCharacter::StunEnd);
}

void AUPPettuCharacter::PlayPatternMontage(UAnimMontage* Montage)
{
	PatternMontage = Montage;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance == nullptr)
	{
		return;
	}
	AnimInstance->StopAllMontages(0.0f);
	if (PatternMontage)
	{
		AnimInstance->Montage_Play(PatternMontage, 1.0f);
		AnimInstance->OnMontageEnded.AddDynamic(this, &AUPPettuCharacter::PatternMontageEnd);
	}
}

void AUPPettuCharacter::PatternMontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	GetMesh()->GetAnimInstance()->OnMontageEnded.RemoveDynamic(this, &AUPPettuCharacter::PatternMontageEnd);

	AUPPlayerController* PlayerController = Cast<AUPPlayerController>(GetController());
	if (PlayerController && PlayerController->GetHUD())
	{
		UUPHudWidget* HudWidget = Cast<UUPHudWidget>(PlayerController->GetHUD());
		SetupHUDWidget(HudWidget);
	}

	if (PlayerController == nullptr)
	{
		return;
	}
	AUPPlayerController* PlayerController2 = Cast<AUPPlayerController>(PlayerController);
	PlayerController2->HudWidgetObject->SetPettuHudVisible(true);
}

void AUPPettuCharacter::AttackHitCheck()
{
	check(Weapon != nullptr);
	AUPPettuWeapon* PettuWeapon = Cast<AUPPettuWeapon>(Weapon);
	if (PettuWeapon)
	{
		//PettuWeapon->CheckAttackRange(CurrentSkillData);
	}
}

void AUPPettuCharacter::AttackHitCheck(bool bIsAttached, FName SocketName, USkeletalMeshComponent* MeshComp)
{
	check(Weapon != nullptr);
	AUPPettuWeapon* PettuWeapon = Cast<AUPPettuWeapon>(Weapon);
	if (PettuWeapon && bIsAttached && SocketName != NAME_None)
	{
		PettuWeapon->CheckAttackSocket(SocketName, CurrentSkillData, MeshComp);
	}
}

void AUPPettuCharacter::AttackHitCheck(bool bIsAttached, FName SocketName, USkeletalMeshComponent* MeshComp,
	float AttackRange, float Amount, FVector CollisionLocation)
{
	check(Weapon != nullptr);
	AUPPettuWeapon* PettuWeapon = Cast<AUPPettuWeapon>(Weapon);
	if (PettuWeapon && bIsAttached && SocketName != NAME_None)
	{
		PettuWeapon->CheckAttackRange(SocketName, MeshComp, AttackRange, CollisionLocation);
	}
	else if (PettuWeapon && !bIsAttached)
	{
		PettuWeapon->CheckAttackRange(AttackRange, Amount, CollisionLocation);
	}
}


void AUPPettuCharacter::SetupHUDWidget(UUPHudWidget* InHUDWidget)
{
	Super::SetupHUDWidget(InHUDWidget);
	if (InHUDWidget)
	{
		InHUDWidget->SetPettuCharacter(this);
	}
}

void AUPPettuCharacter::StunCheck(float Hp)
{
	float CurrentHp = StatComponent->GetCurrentHp();
	float CurrentStunStack = StatComponent->GetCurrentStunStack();
	float MaxHp = StatComponent->GetBaseStat().MaxHp;
	float MaxStunStack = StatComponent->GetBaseStat().MaxStunStack;
	//float HealingHp = (MaxHp / MaxStunStack) * (CurrentStunStack + 1) - CurrentHp;
	if (CurrentHp <= (MaxHp / MaxStunStack) * (CurrentStunStack - 1) && CurrentHp > 0.0f)
	{
		SetStun();
		//StatComponent->HealHp(HealingHp);
	}
}

void AUPPettuCharacter::StunEnd(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage != StunMontage)
	{
		return;
	}
	bIsStun = false;
	if (MonsterAIController)
	{
		UBlackboardComponent* BlackboardComp = MonsterAIController->GetBlackboardComponent();
		if (BlackboardComp)
		{
			BlackboardComp->SetValueAsBool(TEXT("IsStun"), bIsStun);
		}
	}
	GetMesh()->GetAnimInstance()->OnMontageEnded.RemoveDynamic(this, &AUPPettuCharacter::StunEnd);
}

void AUPPettuCharacter::SetStiffen()
{
	GetCharacterMovement()->SetMovementMode(MOVE_None);
	PlayStiffenAnimation();
	bIsStiffen = true;
	
	if (MonsterAIController)
	{
		UBlackboardComponent* BlackboardComp = MonsterAIController->GetBlackboardComponent();
		if (BlackboardComp)
		{
			BlackboardComp->SetValueAsBool(TEXT("IsStiffen"), bIsStiffen);
		}
	}
	GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &AUPPettuCharacter::StiffenEnd);
}

void AUPPettuCharacter::StiffenEnd(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage != StiffenMontage)
	{
		return;
	}
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	bIsStiffen = false;
	if (MonsterAIController)
	{
		UBlackboardComponent* BlackboardComp = MonsterAIController->GetBlackboardComponent();
		if (BlackboardComp)
		{
			BlackboardComp->SetValueAsBool(TEXT("IsStiffen"), bIsStiffen);
		}
	}
	GetMesh()->GetAnimInstance()->OnMontageEnded.RemoveDynamic(this, &AUPPettuCharacter::StiffenEnd);
}


void AUPPettuCharacter::PlayStiffenAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance == nullptr)
	{
		return;
	}
	AnimInstance->StopAllMontages(0.0f);
	if (StiffenMontage)
	{
		AnimInstance->Montage_Play(StiffenMontage, 1.0f);
	}
}
