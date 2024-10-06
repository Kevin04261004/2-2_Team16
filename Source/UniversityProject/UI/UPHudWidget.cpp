// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UPHudWidget.h"
#include "Components/ProgressBar.h"


void UUPHudWidget::SetPlayerHealth(float CurrentHP, float MaxHP)
{
	if (PlayerHealthBar)
	{
		PlayerHealthBar->SetPercent(CurrentHP / MaxHP);
	}
}

void UUPHudWidget::SetPettuHealth(float CurrentHP, float MaxHP)
{
	if (PettuHealthBar)
	{
		PettuHealthBar->SetPercent(CurrentHP / MaxHP);
	}
}
