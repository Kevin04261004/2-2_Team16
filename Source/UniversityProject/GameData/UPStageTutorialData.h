// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Game/Stage/UPTutorialStage.h"
#include "UPStageTutorialData.generated.h"

/**
 * 
 */
UCLASS()
class UNIVERSITYPROJECT_API UUPStageTutorialData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stage)
	TArray<FUPTutorialStage> TutorialStages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Init")
	TMap<EStageConditionType, FString> TutorialConditionDescriptionMap;
};
