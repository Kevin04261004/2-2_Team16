// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/UPPettuCharacter.h"
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
	
	DistanceFromPlayer = 0.0f;
	DamageReceived = 1.0f;
	hasStatus = PettuStatus::Idle;
	
	StatComponent->ApplyStunStack(101.0f); // 내가 쓰는 방식이 틀렸나?
	// 죽는 몽타주 재설정
	// static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Game/Assets/Ancient_Golem/Demo/ThirdPersonRun_Montage.ThirdPersonRun_Montage"));
	// check(DeadMontageRef.Object != nullptr);
	// DeadMontage = DeadMontageRef.Object;
}

void AUPPettuCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	StatComponent->OnHpZero.AddUObject(this, &AUPPettuCharacter::SetDead);
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

void AUPPettuCharacter::SetPettuStun()
{
	SetStun();
	hasStatus = PettuStatus::Stunned;
}


