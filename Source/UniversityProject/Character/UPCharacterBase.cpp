// Fill out your copyright notice in the Description page of Project Settings.


#include "UPCharacterBase.h"

#include "Components/UPCharacterStatComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/UPComboAttackComponent.h"
#include "Engine/DamageEvents.h"
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
	ComboAttack->OnComboAttackFinish.AddUObject(Weapon, &AUPWeapon::ComboStepEnd);
	ComboAttack->OnComboStepEnd.AddUObject(Weapon, &AUPWeapon::ComboStepEnd);
	Weapon->OnWeaponHit.AddUObject(this, &AUPCharacterBase::Attack);
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
	Weapon->CheckAttackRange();
}

void AUPCharacterBase::GoForward()
{
	IUPCharacterGoForwardInterface::GoForward();

	CollisionComponent = Cast<UPrimitiveComponent>(GetRootComponent());
	CollisionComponent->SetSimulatePhysics(true);
	if (!TryCheckForwardCollision(GoForwardDistance / 3.f))
	{
		CollisionComponent->AddImpulse(GetActorForwardVector() * GoForwardDistance,"", true);
	}
	GetWorld()->GetTimerManager().SetTimer(PhysicsTimerHandle, this, &AUPCharacterBase::SetPhysicsFalse, 0.2f, false);
}

bool AUPCharacterBase::TryCheckForwardCollision(float InLineTraceDistance)
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

	DrawDebugLine(GetWorld(), Start, End, bHit ? FColor::Green : FColor::Red, false, 1, 0, 1);

	return bHit;
}

void AUPCharacterBase::SetPhysicsFalse()
{
	if (CollisionComponent != nullptr)
	{
		CollisionComponent->SetSimulatePhysics(false);
	}
}

float AUPCharacterBase::UPTakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	Stat->ApplyDamage(DamageAmount);
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
	Damageable->UPTakeDamage(Stat->GetTotalStat().AttackDamage, DamageEvent, GetController(), this);
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
