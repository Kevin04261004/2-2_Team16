// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/UPPettuCharacter.h"
#include "AI/UPPettuAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/GameModeBase.h"
#include "Character/Weapon/UPPettuWeapon.h"
#include "Components/UPCharacterStatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interface/UPGameInterface.h"
#include "Skill/UPSkillBase.h"
#include "UI/UPHudWidget.h"

AUPPettuCharacter::AUPPettuCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UUPCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	static ConstructorHelpers::FObjectFinder<UUPCharacterStatData> StatDataRef(TEXT("/Game/UniversityProject/GameData/DA_PettuCharacterStat.DA_PettuCharacterStat"));
	StatComponent->SetBaseStat(StatDataRef.Object.Get()->Stat);
	
	MaxComboCount = 3.0f;
	BaseComboFrameRate = 60.f;
	LastComboFrameRate = 120.0f;
	DamageReceived = 1.0f;
	bIsStiffen = false;
	bIsStun = false;
	bIsDead = false;

	InitSkillMap();
}

void AUPPettuCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//StatComponent->OnHpZero.AddUObject(this, &AUPPettuCharacter::SetDead);
	StatComponent->OnStunStackZero.AddUObject(this, &AUPPettuCharacter::SetStun);
	StatComponent->OnStiffen.AddUObject(this, &AUPPettuCharacter::SetStiffen);
	PettuAIController = Cast<AUPPettuAIController>(GetController());
}

void AUPPettuCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = StatComponent->GetBaseStat().WalkSpeed;
	
	CreateDefaultObjectSkill();
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

void AUPPettuCharacter::InitSkillMap()
{
	const UEnum* EnumPtr = StaticEnum<EPettuSkillType>();
	if (!EnumPtr)
	{
		return;
	}
	
	for (int32 i = 0; i < EnumPtr->GetMaxEnumValue(); ++i)  // GetMaxEnumValue()를 사용
	{
		EPettuSkillType EnumValue = static_cast<EPettuSkillType>(EnumPtr->GetValueByIndex(i));  // GetValueByIndex() 사용
		SkillMapInitializer.Add(EnumValue, nullptr);
	}
}

void AUPPettuCharacter::CreateDefaultObjectSkill()
{
	
	for (auto skillMapTuple : SkillMapInitializer)
	{
		if (skillMapTuple.Value == nullptr)
		{
			continue;
		}
		
		if (UUPSkillBase* NewSkillComponent = NewObject<UUPSkillBase>(this, skillMapTuple.Value))
		{
			// 컴포넌트를 월드에 등록합니다.
			NewSkillComponent->RegisterComponent();

			// SkillMap에 새로 생성된 컴포넌트를 추가합니다.
			EPettuSkillType Type = skillMapTuple.Key;
			SkillMap.Add(Type, NewSkillComponent);

			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Skill Map Created"));
		}
	}
}

void AUPPettuCharacter::AttackHitCheck()
{
	check(Weapon != nullptr);
	AUPPettuWeapon* PettuWeapon = Cast<AUPPettuWeapon>(Weapon);
	if (PettuWeapon)
	{
		PettuWeapon->CheckAttackRange(CurrentSkillData);
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

void AUPPettuCharacter::SkillAttack(EPettuSkillType SkillType)
{
	UUPSkillBase** Skill = SkillMap.Find(SkillType);
	CurrentSkillData = Cast<UUPPettuSkillData>((*Skill)->GetSkillData());
	(*Skill)->TryActivateSkill(nullptr);
	CurAttackDamage = (*Skill)->GetSkillAttackDamage();
}

void AUPPettuCharacter::SetupHUDWidget(UUPHudWidget* InHUDWidget)
{
	if (InHUDWidget)
	{
		InHUDWidget->SetPettuCharacter(this);
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


