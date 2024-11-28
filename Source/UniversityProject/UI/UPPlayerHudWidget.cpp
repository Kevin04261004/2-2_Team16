// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UPPlayerHudWidget.h"
#include "Character/UPPlayerCharacter.h"

UUPPlayerHudWidget::UUPPlayerHudWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

float UUPPlayerHudWidget::GetPlayerHpPercent_Implementation() const
{
	if (PlayerCharacter)
	{
		return PlayerCharacter->GetStat()->GetCurrentHp() / PlayerCharacter->GetStat()->GetBaseStat().MaxHp;
	}
	return 0.0f;
}


