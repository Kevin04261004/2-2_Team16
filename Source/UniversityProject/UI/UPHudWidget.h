// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameData/UPCharacterStat.h"
#include "UI/UPUserWidget.h"
#include "UPHudWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNIVERSITYPROJECT_API UUPHudWidget : public UUPUserWidget
{
	GENERATED_BODY()
public:
	UUPHudWidget(const FObjectInitializer& ObjectInitializer);
	
	UFUNCTION(BlueprintCallable, Category = "HUD")
	virtual void SetHealth(float CurrentHP, float MaxHP);

	virtual void UpdateStat(const FUPCharacterStat& BaseStat, const FUPCharacterStat& ModifierStat);
	virtual void UpdateHp(float NewCurrentHp);

protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UProgressBar> HealthBar;

	UPROPERTY()
	float CurrentHp;

	UPROPERTY()
	float MaxHp;
};
