// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/UPPlayerCharacter.h"
#include "Character/Enemy/UPPettuCharacter.h"
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

	void SetPlayerCharacter(AUPPlayerCharacter* InPlayerCharacter) { PlayerCharacter = InPlayerCharacter; }
	void SetPettuCharacter(AUPPettuCharacter* InPettuCharacter) { PettuCharacter = InPettuCharacter; }

protected:
	virtual void NativeConstruct() override;
	
	float PlayerHP = 0.0f;
	float PlayerMaxHP = 0.0f;

	float PettuHP = 0.0f;
	float PettuMaxHP = 0.0f;

	UPROPERTY()
	TObjectPtr<AUPPlayerCharacter> PlayerCharacter;

	UPROPERTY()
	TObjectPtr<AUPPettuCharacter> PettuCharacter;
};
