// Fill out your copyright notice in the Description page of Project Settings.


#include "UPCharacterBase.h"
#include "Components/UPCharacterStatComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/UPCharacterMovementComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/UPHudWidget.h"
#include "GameFramework/HUD.h"
#include "Weapon/UPPlayerCharacterWeapon.h"

// Sets default values
AUPCharacterBase::AUPCharacterBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UUPCharacterMovementComponent>(CharacterMovementComponentName))
{
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

	DiedCondition = EStageConditionType::None;
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
	MovementComponent->Initialize();
	MovementComponent->SetCharacterStat(StatComponent);
	MovementComponent->SetIsSprinting(false);
	
	// Spawn the weapon(Actor) & Get hand Socket to Add it
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	check(WeaponClass != nullptr);
	Weapon = GetWorld()->SpawnActor<AUPWeaponBase>(WeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	check(Weapon != nullptr);
	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("WeaponSocket"));

	/* Actor Delegate */
	Weapon->OnWeaponHit.AddUObject(this, &AUPCharacterBase::Attack);

	/* Subscribe StageManager Delegate */
	StageManager->OnBossStageStart.AddUObject(this, &AUPCharacterBase::SetUI);
	SetUI();
}

void AUPCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

bool AUPCharacterBase::TryCheckForwardCollision(float InLineTraceDistance, FHitResult& OutHit, FVector& OutActorLocation) // 캐릭터 앞에 콜라이더가 존재하는지 확인합니다. 존재하면 true를 리턴합니다.
{
	OutActorLocation = GetActorLocation();
	FVector ActorForwardVector = GetActorForwardVector();

	float CapsuleHalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	bool bHit = false;

	for (int32 i = 0; i < 10; ++i)
	{
		float zPos = FMath::Lerp(OutActorLocation.Z - CapsuleHalfHeight, OutActorLocation.Z + CapsuleHalfHeight, (i / 10.f));
		FVector StartPos = OutActorLocation;
		StartPos.Z = zPos;
		FVector EndPos = (OutActorLocation + (ActorForwardVector * InLineTraceDistance));
		EndPos.Z = zPos;

		bHit = GetWorld()->LineTraceSingleByChannel(
			OutHit,
			StartPos,
			EndPos,
			ECC_Visibility,
			CollisionParams
		);
		if (bHit)
		{
			return true;
		}
	}

	return false;
}

void AUPCharacterBase::Attack(FHitResult& InHit)
{
	IUPDamageableInterface* Damageable = Cast<IUPDamageableInterface>(InHit.GetActor());
	if (Damageable == nullptr)
	{
		return;
	}
	FDamageEvent DamageEvent;
	Damageable->UPTakeDamage(CurAttackDamage, DamageEvent, GetController(), this);
}

void AUPCharacterBase::SetDead()
{
	Weapon->Destroy();
	Super::SetDead();
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

void AUPCharacterBase::SetUI()
{
	UWorld* World = GetWorld();
	if (World)
	{
		// 현재 PlayerController에서 Viewport에 있는 모든 위젯을 탐색
		for (TObjectIterator<UUserWidget> It; It; ++It)
		{
			UUPHudWidget* HudWidget = Cast<UUPHudWidget>(*It);
			if (HudWidget)
			{
				SetupHUDWidget(HudWidget);
			}
		}
	}
}

void AUPCharacterBase::DestroyWeapon()
{
	if (IsValid(Weapon))
	{
		Weapon->Destroy();
	}
}
