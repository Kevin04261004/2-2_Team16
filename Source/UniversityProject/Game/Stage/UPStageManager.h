// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UPTutorialStage.h"
#include "GameData/UPStageTutorialData.h"
#include "GameFramework/Actor.h"
#include "UPStageManager.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnStageClear, int /* StageCount */)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnStageStart, int /* StageCount */)

UCLASS()
class UNIVERSITYPROJECT_API AUPStageManager : public AActor
{
	GENERATED_BODY()
	
public:
	AUPStageManager();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Init")
	TObjectPtr<UUPStageTutorialData> StageTutorialData;

	UPROPERTY(BlueprintReadOnly)
	int32 CurrentStageIndex = 0;

	
	
	void EvaluateCondition(EStageConditionType ConditionType);

	FOnStageClear OnStageClear;
	FOnStageStart OnStageStart;
	
private:
	void EvaluateAllConditions();
	void AdvanceStage();
};

