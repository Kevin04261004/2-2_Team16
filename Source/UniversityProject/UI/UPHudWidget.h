// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UPUserWidget.h"
#include "UPHudWidget.generated.h"

enum class EPlayerExpressionType : uint8;
/**
 * 
 */
UCLASS()
class UNIVERSITYPROJECT_API UUPHudWidget : public UUPUserWidget
{
	GENERATED_BODY()
public:
	UUPHudWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	void SetPlayerHudWidget(class UUPPlayerHudWidget* InPlayerHudWidget) { PlayerHudWidget = InPlayerHudWidget; }
	UFUNCTION(BlueprintCallable)
	void SetPettuHudWidget(class UUPPettuHudWidget* InPettuHudWidget) { PettuHudWidget = InPettuHudWidget; }
	
	void SetPlayerCharacter(class AUPPlayerCharacter* InPlayerCharacter) { PlayerCharacter = InPlayerCharacter; }
	void SetPettuCharacter(class AUPPettuCharacter* InPettuCharacter) { PettuCharacter = InPettuCharacter; }
	
	void SetPlayerHudVisible(bool bVisible);
	void SetPettuHudVisible(bool bVisible);

	void SetExpression(EPlayerExpressionType expressionType);

protected:
	UPROPERTY()
	TObjectPtr<class AUPPlayerCharacter> PlayerCharacter;

	UPROPERTY()
	TObjectPtr<class AUPPettuCharacter> PettuCharacter;

private:
	TObjectPtr<class UUPPlayerHudWidget> PlayerHudWidget;
	
	TObjectPtr<class UUPPettuHudWidget> PettuHudWidget;
};
