// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Stage/UPStageManager.h"

// Sets default values
AUPStageManager::AUPStageManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AUPStageManager::AdvanceStage()
{
	if (CurrentStageIndex < StageTutorialData->TutorialStages.Num() - 1)
	{
		OnStageClear.Broadcast(CurrentStageIndex);
		CurrentStageIndex++;
		OnStageStart.Broadcast(CurrentStageIndex);
		UE_LOG(LogTemp, Log, TEXT("스테이지 진행: %d"), CurrentStageIndex);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("튜토리얼 완료!"));
	}
}

void AUPStageManager::EvaluateCondition(EStageConditionType ConditionType)
{
	if (StageTutorialData->TutorialStages.IsValidIndex(CurrentStageIndex))
	{
		FUPTutorialStage& CurrentStage = StageTutorialData->TutorialStages[CurrentStageIndex];
		
		if (CurrentStage.StageConditionMap.Find(ConditionType))
		{
			CurrentStage.StageConditionMap[ConditionType]--;

			EvaluateAllConditions();
		}
	}
}

void AUPStageManager::EvaluateAllConditions()
{
	FUPTutorialStage& CurrentStage = StageTutorialData->TutorialStages[CurrentStageIndex];
	for (TTuple<EStageConditionType, int> condition : CurrentStage.StageConditionMap)
	{
		if (condition.Value > 0)
		{
			return;
		}
	}

	AdvanceStage();
}
