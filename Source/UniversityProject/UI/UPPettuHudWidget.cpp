// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UPPettuHudWidget.h"
#include "Components/ProgressBar.h"
#include "Interface/UPCharacterHUDInterface.h"


void UUPPettuHudWidget::SetHealth(float CurrentHP, float MaxHP)
{
	//Super::SetHealth(CurrentHP, MaxHP);
	if (HealthBar)
	{
		HealthBar->SetPercent(CurrentHP / MaxHP);
	}
}

void UUPPettuHudWidget::UpdateStat(const FUPCharacterStat& BaseStat, const FUPCharacterStat& ModifierStat)
{
	//Super::UpdateStat(BaseStat, ModifierStat);
	MaxHp = (BaseStat + ModifierStat).MaxHp;
	CurrentHp = (BaseStat + ModifierStat).MaxHp;
	if (HealthBar)
	{
		HealthBar->SetPercent(CurrentHp / MaxHp);
	}
}

void UUPPettuHudWidget::UpdateHp(float NewCurrentHp)
{
	//Super::UpdateHp(NewCurrentHp);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("UUPPettuHudWidget::UpdateHp"));
	CurrentHp = NewCurrentHp;
	ensure(MaxHp > 0.0f);
	if (HealthBar)
	{
		HealthBar->SetPercent(CurrentHp / MaxHp);
	}
}

void UUPPettuHudWidget::NativeConstruct()
{
	//Super::NativeConstruct();
	IUPCharacterHUDInterface* HUDPawn = Cast<IUPCharacterHUDInterface>(GetOwningPlayerPawn());
	if (HUDPawn)
	{
		HUDPawn->SetupHUDWidget(this);
	}
}
