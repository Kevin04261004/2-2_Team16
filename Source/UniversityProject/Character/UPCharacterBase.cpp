// Fill out your copyright notice in the Description page of Project Settings.


#include "UPCharacterBase.h"
#include "Components/UPCharacterStatComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/UPCharacterMovementComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapon/UPPlayerCharacterWeapon.h"

// Sets default values
AUPCharacterBase::AUPCharacterBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UUPCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
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
	MovementComponent = Cast<UUPCharacterMovementComponent>(GetCharacterMovement());
	MovementComponent->bOrientRotationToMovement = true;
	MovementComponent->RotationRate = FRotator(0.0f, 600.0f, 0.0f);
	MovementComponent->JumpZVelocity = 700.f;
	MovementComponent->AirControl = 0.35f;
	MovementComponent->MaxWalkSpeed = 500.f;
	MovementComponent->MinAnalogWalkSpeed = 20.f;
	MovementComponent->BrakingDecelerationWalking = 2000.f;
	
	// Mesh
	TObjectPtr<USkeletalMeshComponent> MeshComponent = GetMesh();
	MeshComponent->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	MeshComponent->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	MeshComponent->SetCollisionProfileName(TEXT("NoCollision"));

	// Set Stat
	StatComponent = CreateDefaultSubobject<UUPCharacterStatComponent>(TEXT("Stat"));

	bIsDead = false;
}

void AUPCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	/* Check InitSection */
	check(DeadMontage != nullptr);
	
	/* Init */
	// Set AnimInstance
	GetMesh()->SetAnimInstanceClass(AnimInstanceClass);

	// Set StatComponent Value
	check(StatComponent != nullptr);
	check(CharacterInitalizeStatData != nullptr);	
	StatComponent->SetBaseStat(CharacterInitalizeStatData->Stat);
	MovementComponent->SetCharacterStat(StatComponent);
    MovementComponent->SetIsSprinting(false);
	
	// Spawn the weapon(Actor) & Get hand Socket to Add it
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	check(WeaponClass != nullptr);
	Weapon = GetWorld()->SpawnActor<AUPWeaponBase>(WeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	check(Weapon != nullptr);
	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("hand_rSocket"));

	/* Actor Delegate */
	Weapon->OnWeaponHit.AddUObject(this, &AUPCharacterBase::Attack);
}

void AUPCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	/* Component Delegate */
	check(StatComponent != nullptr);
	StatComponent->OnHpZero.AddUObject(this, &AUPCharacterBase::SetDead);
	StatComponent->OnStatChanged.AddUObject(this, &AUPCharacterBase::ApplyStat);
}

bool AUPCharacterBase::TryCheckForwardCollision(float InLineTraceDistance, FHitResult& OutHit) // 캐릭터 앞에 콜라이더가 존재하는지 확인합니다. 존재하면 true를 리턴합니다.
{
	FVector Start = GetActorLocation();
	FVector ForwardVector = GetActorForwardVector();
	FVector End = (Start + (ForwardVector * InLineTraceDistance));
	
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		OutHit,
		Start,
		End,
		ECC_Visibility,
		CollisionParams
	);

	return bHit;
}

float AUPCharacterBase::UPTakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	StatComponent->ApplyDamage(DamageAmount);
	return DamageAmount;
}

void AUPCharacterBase::Attack(FHitResult& InHit)
{
	IUPDamageableInterface* Damageable = Cast<IUPDamageableInterface>(InHit.GetActor());
	if (Damageable == nullptr)
	{
		return;
	}
	FDamageEvent DamageEvent;
	Damageable->UPTakeDamage(StatComponent->GetTotalStat().AttackDamage, DamageEvent, GetController(), this);
}

void AUPCharacterBase::SetDead()
{
	GetCharacterMovement()->SetMovementMode(MOVE_None);
	PlayDeadAnimation();
	SetActorEnableCollision(false);
	bIsDead = true;
}

void AUPCharacterBase::PlayDeadAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance == nullptr)
	{
		return;
	}
	AnimInstance->StopAllMontages(0.0f);
	check(DeadMontage != nullptr);
	AnimInstance->Montage_Play(DeadMontage, 1.0f);
	AnimInstance->OnMontageEnded.AddDynamic(this, &AUPCharacterBase::DeadAnimEnd);
}

void AUPCharacterBase::DeadAnimEnd(UAnimMontage* Montage, bool bInterrupted)
{
	GetMesh()->GetAnimInstance()->OnMontageEnded.RemoveDynamic(this, &AUPCharacterBase::DeadAnimEnd);
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	bIsDead = false;
}

void AUPCharacterBase::ApplyStat(const FUPCharacterStat& BaseStat, const FUPCharacterStat& ModifierStat)
{
	MovementComponent->SetIsSprinting(MovementComponent->GetIsSprinting());
}

void AUPCharacterBase::SetStun()
{
	GetCharacterMovement()->SetMovementMode(MOVE_None);
	PlayStunAnimation();
	bIsStun = true;
}

void AUPCharacterBase::PlayStunAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance == nullptr)
	{
		return;
	}
	AnimInstance->StopAllMontages(0.0f);
	if (StunMontage)
	{
		AnimInstance->Montage_Play(StunMontage, 1.0f);
		AnimInstance->OnMontageEnded.AddDynamic(this, &AUPCharacterBase::StunAnimEnd);
	}
}

void AUPCharacterBase::StunAnimEnd(UAnimMontage* Montage, bool bInterrupted)
{
	GetMesh()->GetAnimInstance()->OnMontageEnded.RemoveDynamic(this, &AUPCharacterBase::StunAnimEnd);
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	bIsStun = false;
}
