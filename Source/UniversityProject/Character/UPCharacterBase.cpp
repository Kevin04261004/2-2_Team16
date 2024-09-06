// Fill out your copyright notice in the Description page of Project Settings.


#include "UPCharacterBase.h"

#include "Components/UPCharacterStatComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/UPComboAttackComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AUPCharacterBase::AUPCharacterBase()
{
	// Pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Capsule
	// TObjectPtr<UCapsuleComponent> CapsuleComponent = GetCapsuleComponent();
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName("Capsule");
	
	// Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	// Set Stat
	Stat = CreateDefaultSubobject<UUPCharacterStatComponent>(TEXT("Stat"));
	// InitializeStat();

	// Set Combo
	ComboAttack = CreateDefaultSubobject<UUPComboAttackComponent>(TEXT("Combo Attack"));
}

void AUPCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Stat->OnHpZero.AddUObject(this, &AUPCharacterBase::SetDead);
	Stat->OnStatChanged.AddUObject(this, &AUPCharacterBase::ApplyStat);
}

float AUPCharacterBase::UPTakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Stat->ApplyDamage(DamageAmount);
	return 0;
}

void AUPCharacterBase::SetDead()
{
	GetCharacterMovement()->SetMovementMode(MOVE_None);
	PlayDeadAnimation();
	SetActorEnableCollision(false);
}

void AUPCharacterBase::PlayDeadAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(DeadMontage, 1.0f);
}

int32 AUPCharacterBase::GetLevel()
{
	return Stat->GetCurrentLevel();
}

void AUPCharacterBase::SetLevel(int32 InNewLevel)
{
	Stat->SetLevelStat(InNewLevel);
}

void AUPCharacterBase::ApplyStat(const FUPCharacterStat& BaseStat, const FUPCharacterStat& ModifierStat)
{
	float MovementSpeed = (BaseStat + ModifierStat).MovementSpeed;
	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
}
