// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/UPDamagableActor.h"

#include "Components/UPCharacterStatComponent.h"
#include "GameData/UPCharacterStatData.h"

// Sets default values
AUPDamagableActor::AUPDamagableActor()
{
	
	Stat = CreateDefaultSubobject<UUPCharacterStatComponent>(TEXT("Stat"));
	static ConstructorHelpers::FObjectFinder<UUPCharacterStatData> StatDataRef(TEXT("/Game/UniversityProject/GameData/DA_PlayerCharacterStat.DA_PlayerCharacterStat"));
	Stat->SetBaseStat(StatDataRef.Object.Get()->Stat);
}

float AUPDamagableActor::UPTakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	Stat->ApplyDamage(DamageAmount);
	return DamageAmount;
}

void AUPDamagableActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Stat->OnHpZero.AddUObject(this, &AUPDamagableActor::SetDead);
	Stat->OnStatChanged.AddUObject(this, &AUPDamagableActor::ApplyStat);
}

void AUPDamagableActor::SetDead()
{
	SetActorEnableCollision(false);
	Destroy();
}

void AUPDamagableActor::ApplyStat(const FUPCharacterStat& BaseStat, const FUPCharacterStat& ModifierStat)
{
	
}
