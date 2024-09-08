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

	// Set Stat
	Stat = CreateDefaultSubobject<UUPCharacterStatComponent>(TEXT("Stat"));
	static ConstructorHelpers::FObjectFinder<UUPCharacterStatData> StatDataRef(TEXT("/Game/UniversityProject/GameData/DA_PlayerCharacterStat.DA_PlayerCharacterStat"));
	Stat->SetBaseStat(StatDataRef.Object.Get()->Stat);
	
	// Set Combo
	ComboAttack = CreateDefaultSubobject<UUPComboAttackComponent>(TEXT("Combo Attack"));

	// Get Socket And Add it
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));

	/* How do we get socket transform */
	// 1. 매 프레임 마다 검 휘두루는 애니메이션에서 Notify를 통해 검의 위치를 가져오는 함수를 실행!

	// 2. 해당 함수를 대충 구현해본 내용 ▼
	if (Weapon != nullptr)
	{
		/* 소켓 이름 */
		FName SocketName_1 = FName(TEXT("1"));
		FName SocketName_2 = FName(TEXT("2"));
		FName SocketName_3 = FName(TEXT("3"));

		/* 소켓이 존재하는지 assert! */
		// check (Weapon->DoesSocketExist(SocketName_1));
		// check (Weapon->DoesSocketExist(SocketName_2));
		// check (Weapon->DoesSocketExist(SocketName_3));

		/* 소켓의 위치를 가져옵니다. */
		FVector vec_1 = Weapon->GetSocketLocation(SocketName_1);
		FVector vec_2 = Weapon->GetSocketLocation(SocketName_2);
		FVector vec_3 = Weapon->GetSocketLocation(SocketName_3);

		// Draw It! (Debug)
		DrawDebugLine(GetWorld(), vec_1, vec_2, FColor::Green, false, 0.1f, 0, 2.0f);
		DrawDebugLine(GetWorld(), vec_2, vec_3, FColor::Green, false, 0.1f, 0, 2.0f);
		DrawDebugLine(GetWorld(), vec_3, vec_1, FColor::Green, false, 0.1f, 0, 2.0f);
		
		// Create Collsion
		
	}
	
	
	// Set AnimInstance
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/UniversityProject/Animation/AUP_UPCharacter.AUP_UPCharacter_C"));
	check(AnimInstanceClassRef.Class != nullptr);
	GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);

	// Set Dead Montage
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Game/UniversityProject/Animation/AM_Dead.AM_Dead"));
	check(DeadMontageRef.Object != nullptr);
	DeadMontage = DeadMontageRef.Object;
}

void AUPCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Stat->OnHpZero.AddUObject(this, &AUPCharacterBase::SetDead);
	Stat->OnStatChanged.AddUObject(this, &AUPCharacterBase::ApplyStat);
}

void AUPCharacterBase::NotifyComboActionEnd()
{
	
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
