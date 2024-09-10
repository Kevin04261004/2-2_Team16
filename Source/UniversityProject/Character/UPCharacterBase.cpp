// Fill out your copyright notice in the Description page of Project Settings.


#include "UPCharacterBase.h"

#include "Components/UPCharacterStatComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/UPComboAttackComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapon/UPWeapon.h"

// Sets default values
AUPCharacterBase::AUPCharacterBase()
{
	// Pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Capsule
	TObjectPtr<UCapsuleComponent> CapsuleComponent_ = GetCapsuleComponent();
	CapsuleComponent_->InitCapsuleSize(42.f, 96.0f);
	CapsuleComponent_->SetCollisionProfileName("Capsule");
	
	// Movement
	TObjectPtr<UCharacterMovementComponent> CharacterMovementComponent = GetCharacterMovement();
	CharacterMovementComponent->bOrientRotationToMovement = true;
	CharacterMovementComponent->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	CharacterMovementComponent->JumpZVelocity = 700.f;
	CharacterMovementComponent->AirControl = 0.35f;
	CharacterMovementComponent->MaxWalkSpeed = 500.f;
	CharacterMovementComponent->MinAnalogWalkSpeed = 20.f;
	CharacterMovementComponent->BrakingDecelerationWalking = 2000.f;

	// Mesh
	TObjectPtr<USkeletalMeshComponent> MeshComponent = GetMesh();
	MeshComponent->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	MeshComponent->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	MeshComponent->SetCollisionProfileName(TEXT("NoCollision"));

	// Set AnimInstance
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/UniversityProject/Animation/AUP_UPCharacter.AUP_UPCharacter_C"));
	check(AnimInstanceClassRef.Class != nullptr);
	GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	
	// Set Stat
	Stat = CreateDefaultSubobject<UUPCharacterStatComponent>(TEXT("Stat"));
	static ConstructorHelpers::FObjectFinder<UUPCharacterStatData> StatDataRef(TEXT("/Game/UniversityProject/GameData/DA_PlayerCharacterStat.DA_PlayerCharacterStat"));
	Stat->SetBaseStat(StatDataRef.Object.Get()->Stat);
	
	// Set Combo
	ComboAttack = CreateDefaultSubobject<UUPComboAttackComponent>(TEXT("Combo Attack"));

	// Set Dead Montage
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Game/UniversityProject/Animation/AM_Dead.AM_Dead"));
	check(DeadMontageRef.Object != nullptr);
	DeadMontage = DeadMontageRef.Object;
}

void AUPCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	// Spawn the weapon actor And Get Socket to Add it
	check(WeaponClass != nullptr);
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();

	Weapon = GetWorld()->SpawnActor<AUPWeapon>(WeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	check(Weapon != nullptr);
	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("hand_rSocket"));
}

void AUPCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Stat->OnHpZero.AddUObject(this, &AUPCharacterBase::SetDead);
	Stat->OnStatChanged.AddUObject(this, &AUPCharacterBase::ApplyStat);
}

void AUPCharacterBase::AttackHitCheck()
{
	check(Weapon != nullptr);
	Weapon->NotifyAttackCheck();
}

void AUPCharacterBase::AttackComboEnd()
{
	check(Weapon != nullptr);
	Weapon->NotifyAttackComboEnd();
}

void AUPCharacterBase::NotifyComboActionEnd()
{
	check(Weapon != nullptr);
	Weapon->NotifyAttackEnd();
}

void AUPCharacterBase::NotifyAttackComboEnd()
{
	AttackComboEnd();
	NotifyComboActionEnd();
}

float AUPCharacterBase::UPTakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	Stat->ApplyDamage(DamageAmount);
	return DamageAmount;
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

void AUPCharacterBase::ApplyStat(const FUPCharacterStat& BaseStat, const FUPCharacterStat& ModifierStat)
{
	float MovementSpeed = (BaseStat + ModifierStat).MovementSpeed;
	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
}
