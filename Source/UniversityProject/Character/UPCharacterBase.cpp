// Fill out your copyright notice in the Description page of Project Settings.


#include "UPCharacterBase.h"

// Sets default values
AUPCharacterBase::AUPCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AUPCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AUPCharacterBase::AttackHitCheck()
{
}

float AUPCharacterBase::UPTakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{

	return 0;
}

void AUPCharacterBase::SetDead()
{
}

void AUPCharacterBase::PlayDeadAnimation()
{
}

int32 AUPCharacterBase::GetLevel()
{
	return 1;
}

void AUPCharacterBase::SetLevel(int32 InNewLevel)
{
}
