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

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "UI")
	float GetPlayerHpPercent() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "UI")
	float GetPettuHpPercent() const;
	float GetPettuHpPercent_Implementation() const;
	void SetPlayerCharacter(class AUPPlayerCharacter* InPlayerCharacter) { PlayerCharacter = InPlayerCharacter; }
	void SetPettuCharacter(class AUPPettuCharacter* InPettuCharacter) { PettuCharacter = InPettuCharacter; }

protected:
	float PlayerHP = 0.0f;
	float PlayerMaxHP = 0.0f;

	float PettuHP = 0.0f;
	float PettuMaxHP = 0.0f;

	UPROPERTY()
	TObjectPtr<class AUPPlayerCharacter> PlayerCharacter;

	UPROPERTY()
	TObjectPtr<class AUPPettuCharacter> PettuCharacter;
};
