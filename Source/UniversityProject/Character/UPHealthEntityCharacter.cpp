// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/UPHealthEntityCharacter.h"
#include "Components/UPCharacterMovementComponent.h"
#include "Game/UPGameMode.h"

AUPHealthEntityCharacter::AUPHealthEntityCharacter(const FObjectInitializer& ObjectInitializer) : CurAttackDamage(0), bIsDead(false)
{
	// Pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	// Set Stat
	StatComponent = CreateDefaultSubobject<UUPCharacterStatComponent>(TEXT("Stat"));

	DiedCondition = EStageConditionType::KillHealthMeshObject;
	bIsInvincible = false;
}

void AUPHealthEntityCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(StatComponent != nullptr);
	check(CharacterInitalizeStatData != nullptr);	
	StatComponent->SetBaseStat(CharacterInitalizeStatData->Stat);

	AGameModeBase* GM = GetWorld()->GetAuthGameMode();
	if (GM == nullptr)
	{
		return;
	}
	AUPGameMode* gameMode = Cast<AUPGameMode>(GM);
	if (gameMode == nullptr)
	{
		return;
	}
	StageManager = gameMode->StageManager;
	check(StageManager != nullptr);
}

void AUPHealthEntityCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	check(StatComponent != nullptr);
	StatComponent->OnHpZero.AddUObject(this, &AUPHealthEntityCharacter::SetDead);
}

float AUPHealthEntityCharacter::UPTakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	if (bIsInvincible)
	{
		return 0;
	}
	StatComponent->ApplyDamage(DamageAmount);
	OnTakeDamaged.Broadcast(DamageAmount);
	return DamageAmount;
}

void AUPHealthEntityCharacter::SetDead()
{
	StageManager->EvaluateCondition(DiedCondition);
	GetCharacterMovement()->SetMovementMode(MOVE_None);
	SetActorEnableCollision(false);
	bIsDead = true;
	PlayDeadAnimation();
}

void AUPHealthEntityCharacter::PlayDeadAnimation()
{
	if (DeadMontage == nullptr)
	{
		Destroy();
		return;
	}
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance == nullptr)
	{
		return;
	}
	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(DeadMontage, 1.0f);
	AnimInstance->OnMontageEnded.AddDynamic(this, &AUPHealthEntityCharacter::DeadAnimEnd);
}

void AUPHealthEntityCharacter::DeadAnimEnd(UAnimMontage* Montage, bool bInterrupted)
{
	GetMesh()->GetAnimInstance()->OnMontageEnded.RemoveDynamic(this, &AUPHealthEntityCharacter::DeadAnimEnd);
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	bIsDead = false;
}