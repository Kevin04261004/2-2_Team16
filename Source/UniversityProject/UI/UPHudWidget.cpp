// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UPHudWidget.h"

#include "Character/UPPlayerCharacter.h"
#include "Character/Enemy/UPPettuCharacter.h"

UUPHudWidget::UUPHudWidget(const FObjectInitializer& ObjectInitializer)
{
}

float UUPHudWidget::GetPlayerHpPercent_Implementation() const
{
	if (PlayerCharacter)
	{
		return PlayerCharacter->GetStat()->GetCurrentHp() / PlayerCharacter->GetStat()->GetBaseStat().MaxHp;
	}
	return 0.0f;
}

float UUPHudWidget::GetPettuHpPercent_Implementation() const
{
	if (PettuCharacter)
	{
		return PettuCharacter->GetStat()->GetCurrentHp() / PettuCharacter->GetStat()->GetBaseStat().MaxHp;
	}
	return 0.0f;
}




