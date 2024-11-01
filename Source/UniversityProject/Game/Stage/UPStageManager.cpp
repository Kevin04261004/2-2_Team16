// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Stage/UPStageManager.h"

// Sets default values
AUPStageManager::AUPStageManager()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AUPStageManager::AdvanceStage()
{
	if (CurrentStageIndex < TutorialStages.Num() - 1)
	{
		CurrentStageIndex++;
		UE_LOG(LogTemp, Log, TEXT("스테이지 진행: %d"), CurrentStageIndex);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("튜토리얼 완료!"));
	}
}

void AUPStageManager::EvaluateCondition(EStageConditionType ConditionType)
{
	if (TutorialStages.IsValidIndex(CurrentStageIndex))
	{
		FUPTutorialStage& CurrentStage = TutorialStages[CurrentStageIndex];
		
		if (CurrentStage.ConditionType == ConditionType)
		{
			CurrentStage.bIsComplete = true;
			AdvanceStage();
		}
	}
}