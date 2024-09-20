// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UPComboActionData.generated.h"

/**
 * 
 */
UCLASS()
class UNIVERSITYPROJECT_API UUPComboActionData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UUPComboActionData();

	UPROPERTY(EditAnywhere, Category = Name)
	FString MontageSectionNamePrefix;

	UPROPERTY(EditAnywhere, Category = Name)
	uint8 MaxComboCount;

	UPROPERTY(EditAnywhere, Category = Name)
	float FrameRate;

	UPROPERTY(EditAnywhere, Category = Name)
	TArray<float> EffectiveFrameCount;
};
