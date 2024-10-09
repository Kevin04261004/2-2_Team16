// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UPHudWidget.h"
#include "UPPettuHudWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNIVERSITYPROJECT_API UUPPettuHudWidget : public UUPHudWidget
{
	GENERATED_BODY()
public:
	
	virtual void SetHealth(float CurrentHP, float MaxHP) override;

	virtual void UpdateStat(const FUPCharacterStat& BaseStat, const FUPCharacterStat& ModifierStat) override;
	virtual void UpdateHp(float NewCurrentHp) override;

protected:
	virtual void NativeConstruct() override;
	
};
