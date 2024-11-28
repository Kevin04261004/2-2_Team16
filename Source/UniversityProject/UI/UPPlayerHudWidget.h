// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UPHudWidget.h"
#include "UPPlayerHudWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNIVERSITYPROJECT_API UUPPlayerHudWidget : public UUPHudWidget
{
	GENERATED_BODY()
public:
	UUPPlayerHudWidget(const FObjectInitializer& ObjectInitializer);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "UI")
	float GetPlayerHpPercent() const;

	float GetPlayerHpPercent_Implementation() const;
};
