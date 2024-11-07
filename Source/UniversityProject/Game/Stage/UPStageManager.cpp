// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Stage/UPStageManager.h"

// Sets default values
AUPStageManager::AUPStageManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AUPStageManager::BeginPlay()
{
	Super::BeginPlay();
	
	StartStage(0);
}

void AUPStageManager::EvaluateCondition(EStageConditionType ConditionType)
{
	if (StageTutorialData->TutorialStages.IsValidIndex(CurrentStageIndex))
	{
		if (CurrentStage.StageConditionMap.Find(ConditionType))
		{
			CurrentStage.StageConditionMap[ConditionType]--;

			CheckStageConditions();
		}
	}
}

void AUPStageManager::StartStage(int32 StageIndex)
{
	CurrentStageIndex = StageIndex;
	if (StageTutorialData && StageTutorialData->TutorialStages.IsValidIndex(CurrentStageIndex))
	{
		CurrentStage = StageTutorialData->TutorialStages[CurrentStageIndex];
		StageTutorialData->TutorialStages[CurrentStageIndex].OnStageStart.Broadcast();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("StageTutorialData is invalid or no stages are defined!"));
	}
}

void AUPStageManager::CompleteStage()
{
	UE_LOG(LogTemp, Log, TEXT("Stage %d Complete!"), CurrentStageIndex);
	StageTutorialData->TutorialStages[CurrentStageIndex].OnStageClear.Broadcast();

	CurrentStageIndex++;
	if (StageTutorialData->TutorialStages.IsValidIndex(CurrentStageIndex))
	{
		StartStage(CurrentStageIndex);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("All Stages Completed!"));
	}
}

void AUPStageManager::CheckStageConditions()
{
	bool bAllConditionsMet = true;

	for (const TPair<EStageConditionType, int>& Condition : CurrentStage.StageConditionMap)
	{
		if (Condition.Value > 0)
		{
			bAllConditionsMet = false;
			break;
		}
	}

	if (bAllConditionsMet)
	{
		CompleteStage();
	}
}