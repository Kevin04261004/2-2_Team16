// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UPTutorialStage.h"
#include "GameData/UPStageTutorialData.h"
#include "GameFramework/Actor.h"
#include "UPStageManager.generated.h"

UCLASS()
class UNIVERSITYPROJECT_API AUPStageManager : public AActor
{
	GENERATED_BODY()
	
public:
	AUPStageManager();

	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Init")
	TObjectPtr<UUPStageTutorialData> StageTutorialData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 CurrentStageIndex = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FUPTutorialStage CurrentStage;
	
	void EvaluateCondition(EStageConditionType ConditionType);

private:
	void StartStage(int32 StageIndex);
	void CompleteStage();
	void CheckStageConditions();
};

