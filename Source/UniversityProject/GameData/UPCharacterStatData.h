// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UPCharacterStat.h"
#include "Engine/DataAsset.h"
#include "UPCharacterStatData.generated.h"

/**
 * 
 */
UCLASS()
class UNIVERSITYPROJECT_API UUPCharacterStatData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = Stat)
	FUPCharacterStat Stat;
};
