// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/UPPettuCharacter.h"

#include "AI/UPPettuAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/UPCharacterStatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AUPPettuCharacter::AUPPettuCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UUPCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	static ConstructorHelpers::FObjectFinder<UUPCharacterStatData> StatDataRef(TEXT("/Game/UniversityProject/GameData/DA_PettuCharacterStat.DA_PettuCharacterStat"));
	StatComponent->SetBaseStat(StatDataRef.Object.Get()->Stat);
	GetCharacterMovement()->MaxWalkSpeed = StatComponent->GetBaseStat().WalkSpeed;
	
	MaxComboCount = 3.0f;
	BaseComboFrameRate = 60.0f;
	LastComboFrameRate = 120.0f;
	
	DamageReceived = 1.0f;
	hasStatus = PettuStatus::Idle;
	bIsStiffen = false;

	PettuAIController = Cast<AUPPettuAIController>(GetController());
}

void AUPPettuCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	StatComponent->OnHpZero.AddUObject(this, &AUPPettuCharacter::SetPettuDead);
	StatComponent->OnStunStackZero.AddUObject(this, &AUPPettuCharacter::SetStun);
	StatComponent->OnStiffen.AddUObject(this, &AUPPettuCharacter::SetStiffen);
}

void AUPPettuCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TestHandle, this, &AUPPettuCharacter::TestFunc, 5.5f, false);
}

float AUPPettuCharacter::UPTakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                                      AActor* DamageCauser)
{
	return Super::UPTakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void AUPPettuCharacter::SetPettuDead()
{
	SetDead();
	Destroy();
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
	StatComponent->ApplyStunStack(4.0f);
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


