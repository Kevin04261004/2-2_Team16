// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UPTutorialStage.h"
#include "GameFramework/Actor.h"
#include "UPStageManager.generated.h"

UCLASS()
class UNIVERSITYPROJECT_API AUPStageManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AUPStageManager();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FUPTutorialStage> TutorialStages;

	UPROPERTY(BlueprintReadOnly)
	int32 CurrentStageIndex = 0;

	void AdvanceStage();
	void EvaluateCondition(EStageConditionType ConditionType);
};

