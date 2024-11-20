// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/UPPettuCharacter.h"

#include "BrainComponent.h"
#include "AI/UPPettuAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Weapon/UPPettuWeapon.h"
#include "Components/UPCharacterStatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interface/UPGameInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/MapErrors.h"
#include "UI/UPHudWidget.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"


AUPPettuCharacter::AUPPettuCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UUPCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	static ConstructorHelpers::FObjectFinder<UUPCharacterStatData> StatDataRef(TEXT("/Game/UniversityProject/GameData/DA_PettuCharacterStat.DA_PettuCharacterStat"));
	StatComponent->SetBaseStat(StatDataRef.Object.Get()->Stat);
	
	bIsStiffen = false;
	bIsStun = false;
	bIsDead = false;
}

void AUPPettuCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//StatComponent->OnHpZero.AddUObject(this, &AUPPettuCharacter::SetDead);
	//StatComponent->OnStunStackZero.AddUObject(this, &AUPPettuCharacter::SetStun);
	StatComponent->OnStiffen.AddUObject(this, &AUPPettuCharacter::SetStiffen);
	StatComponent->OnHpChanged.AddUObject(this, &AUPPettuCharacter::StunCheck);
	PettuAIController = Cast<AUPPettuAIController>(GetController());

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
	
	if (NiagaraSystem != nullptr && GetMesh() != nullptr)
	{
		LeftHandEffect = UNiagaraFunctionLibrary::SpawnSystemAttached(
			NiagaraSystem,                        // Niagara 시스템
			GetMesh(),
			TEXT("LeftSocket"),              // 소켓 이름
			FVector::ZeroVector,                 // 위치 (소켓 기준)
			FRotator::ZeroRotator,               // 회전 (소켓 기준)
			EAttachLocation::SnapToTarget,       // 소켓 기준 위치 설정
			false                                 // AutoDestroy 설정
		);
		LeftHandEffect->Deactivate();
		
		RightHandEffect = UNiagaraFunctionLibrary::SpawnSystemAttached(
		NiagaraSystem,                        // Niagara 시스템
		GetMesh(),
		TEXT("RightSocket"),              // 소켓 이름
		FVector::ZeroVector,                 // 위치 (소켓 기준)
		FRotator::ZeroRotator,               // 회전 (소켓 기준)
		EAttachLocation::SnapToTarget,       // 소켓 기준 위치 설정
		false                                 // AutoDestroy 설정
		);
		RightHandEffect->Deactivate();
	}
	
}

float AUPPettuCharacter::UPTakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                                      AActor* DamageCauser)
{
	return Super::UPTakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void AUPPettuCharacter::SetDead()
{
	Super::SetDead();
	MovementComponent->DisableMovement();
	//PettuAIController = Cast<AUPPettuAIController>(GetController());
	if (PettuAIController)
	{
		if (PettuAIController->BrainComponent)
		{
			PettuAIController->BrainComponent->StopLogic(TEXT("Death"));
		}
		PettuAIController->StopMovement();
	}
	
	//Destroy();
}

void AUPPettuCharacter::DeadAnimEnd(UAnimMontage* Montage, bool bInterrupted)
{
	Super::DeadAnimEnd(Montage, bInterrupted);
}

void AUPPettuCharacter::SetStun()
{
	Super::SetStun();
	StatComponent->ApplyStunStack(1);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Stun"));
	if (PettuAIController)
	{
		UBlackboardComponent* BlackboardComp = PettuAIController->GetBlackboardComponent();
		if (BlackboardComp)
		{
			BlackboardComp->SetValueAsBool(TEXT("IsStun"), bIsStun);
		}
	}
	GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &AUPPettuCharacter::StunEnd);
}

void AUPPettuCharacter::TestFunc()
{
	// TODO -> 필요한 기능 테스트용
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
	if (CurrentHp <= (MaxHp / MaxStunStack) * (CurrentStunStack - 1))
	{
		SetStun();
		//StatComponent->HealHp(HealingHp);
	}
}

void AUPPettuCharacter::PunchTrailOn(EPunchTrailType type)
{
	if (type == EPunchTrailType::Left)
	{
		LeftHandEffect->Activate(true);
	}
	else if (type == EPunchTrailType::Right)
	{
		LeftHandEffect->Activate(true);
	}
}

void AUPPettuCharacter::PunchTrailOff(EPunchTrailType type)
{
	if (type == EPunchTrailType::Left)
	{
		LeftHandEffect->Deactivate();
	}
	else if (type == EPunchTrailType::Right)
	{
		LeftHandEffect->Deactivate();
	}
}


void AUPPettuCharacter::StunEnd(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage != StunMontage)
	{
		return;
	}
	bIsStun = false;
	if (PettuAIController)
	{
		UBlackboardComponent* BlackboardComp = PettuAIController->GetBlackboardComponent();
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
	
	if (PettuAIController)
	{
		UBlackboardComponent* BlackboardComp = PettuAIController->GetBlackboardComponent();
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
	if (PettuAIController)
	{
		UBlackboardComponent* BlackboardComp = PettuAIController->GetBlackboardComponent();
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


