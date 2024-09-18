// Fill out your copyright notice in the Description page of Project Settings.


#include "UPCharacterBase.h"

#include "AfterImage/UPAfterImage.h"
#include "Components/UPCharacterStatComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/UPComboAttackComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapon/UPPlayerCharacterWeapon.h"

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
	CharacterMovementComponent->RotationRate = FRotator(0.0f, 600.0f, 0.0f);
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

	// Set Stat
	StatComponent = CreateDefaultSubobject<UUPCharacterStatComponent>(TEXT("Stat"));
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
    GetCharacterMovement()->MaxWalkSpeed = CharacterInitalizeStatData->Stat.WalkSpeed;
	
	// Spawn the weapon(Actor) & Get hand Socket to Add it
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	check(WeaponClass != nullptr);
	Weapon = GetWorld()->SpawnActor<AUPPlayerCharacterWeapon>(WeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
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

bool AUPCharacterBase::TryCheckForwardCollision(float InLineTraceDistance) // 캐릭터 앞에 콜라이더가 존재하는지 확인합니다. 존재하면 true를 리턴합니다.
{
	FVector Start = GetActorLocation();
	FVector ForwardVector = GetActorForwardVector();
	FVector End = (Start + (ForwardVector * InLineTraceDistance));

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
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
}

void AUPCharacterBase::ApplyStat(const FUPCharacterStat& BaseStat, const FUPCharacterStat& ModifierStat)
{
	float MovementSpeed = (BaseStat + ModifierStat).WalkSpeed;
	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
}
