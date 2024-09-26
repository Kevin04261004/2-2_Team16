// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/UPCharacterStatComponent.h"

UUPCharacterStatComponent::UUPCharacterStatComponent()
{
	bWantsInitializeComponent = true;
}

void UUPCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
	
	SetHp(BaseStat.MaxHp);
}

float UUPCharacterStatComponent::ApplyDamage(float InDamage)
{
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage);

	SetHp(PrevHp - ActualDamage);
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
	}
	return ActualDamage;
}

float UUPCharacterStatComponent::ApplyStunStack(float InDamage)
{
	const float PrevSS = CurrentStunStack;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage);

	SetStunStack(PrevSS - ActualDamage);
	if (CurrentStunStack <= KINDA_SMALL_NUMBER)
	{
		OnStunStackZero.Broadcast();
	}
	else if (ActualDamage >= 3)
	{
		OnStiffen.Broadcast();
	}
	return ActualDamage;
}

void UUPCharacterStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, BaseStat.MaxHp);
	OnHpChanged.Broadcast(CurrentHp);
}

void UUPCharacterStatComponent::SetStunStack(float NewStunStat)
{
	CurrentStunStack = FMath::Clamp<float>(NewStunStat, 0.0f, BaseStat.MaxStunStack);
	OnStunStackChanged.Broadcast(CurrentStunStack);
}
