// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UPPettuHudWidget.h"
#include "Character/Enemy/UPPettuCharacter.h"

UUPPettuHudWidget::UUPPettuHudWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

float UUPPettuHudWidget::GetPettuHpPercent_Implementation() const
{
	if (PettuCharacter)
	{
		return PettuCharacter->GetStat()->GetCurrentHp() / PettuCharacter->GetStat()->GetBaseStat().MaxHp;
	}
	return 0.0f;
}
