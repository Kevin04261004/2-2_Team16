// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void SetPlayerHealth(float CurrentHP, float MaxHP);

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void SetPettuHealth(float CurrentHP, float MaxHP);

protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UProgressBar> PlayerHealthBar;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UProgressBar> PettuHealthBar;
};
