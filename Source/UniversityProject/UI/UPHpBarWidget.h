// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameData/UPCharacterStat.h"
#include "UI/UPUserWidget.h"
#include "UPHpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNIVERSITYPROJECT_API UUPHpBarWidget : public UUPUserWidget
{
	GENERATED_BODY()
		
public:
	UUPHpBarWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	void UpdateStat(const FUPCharacterStat& BaseStat, const FUPCharacterStat& ModifierStat);
	void UpdateHpBar(float NewCurrentHp);
	FString GetHpStatText();

protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpProgressBar;

	UPROPERTY()
	TObjectPtr<class UTextBlock> HpStat;

	UPROPERTY()
	float CurrentHp;

	UPROPERTY()
	float MaxHp;
};
