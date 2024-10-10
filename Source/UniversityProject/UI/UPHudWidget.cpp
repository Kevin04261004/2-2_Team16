// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UPHudWidget.h"
#include "Character/Enemy/UPPettuCharacter.h"
#include "Components/ProgressBar.h"
#include "Interface/UPCharacterHUDInterface.h"


UUPHudWidget::UUPHudWidget(const FObjectInitializer& ObjectInitializer)
{
}

void UUPHudWidget::NativeConstruct()
{
	Super::NativeConstruct();

	IUPCharacterHUDInterface* HUDPawn = Cast<IUPCharacterHUDInterface>(GetOwningPlayerPawn());
	if (HUDPawn)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, GetOwningPlayerPawn()->GetName());
		HUDPawn->SetupHUDWidget(this);
	}
}

float UUPHudWidget::GetPlayerHpPercent_Implementation() const
{
	if (PlayerCharacter)
		return PlayerCharacter->GetStatComponent()->GetCurrentHp() / PlayerCharacter->GetStatComponent()->GetBaseStat().MaxHp;
	return 0.0f;
}

float UUPHudWidget::GetPettuHpPercent_Implementation() const
{
	if (PettuCharacter)
	{
		return PettuCharacter->GetStatComponent()->GetCurrentHp() / PettuCharacter->GetStatComponent()->GetBaseStat().MaxHp;
	}
	return 0.0f;
}




