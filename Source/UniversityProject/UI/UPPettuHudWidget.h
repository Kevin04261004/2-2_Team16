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
	UUPPettuHudWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "UI")
	float GetPettuHpPercent() const;

	float GetPettuHpPercent_Implementation() const;
};
