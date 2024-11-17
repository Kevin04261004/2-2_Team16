// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Stage/UPStageManager.h"

#include "Manager/UPActorSpawner.h"

// Sets default values
AUPStageManager::AUPStageManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AUPStageManager::BeginPlay()
{
	Super::BeginPlay();

	UUPActorSpawner* ActorSpawner = GetGameInstance()->GetSubsystem<UUPActorSpawner>();
	if (ActorSpawner != nullptr)
	{
		ActorSpawner->InitializeSpawner(this);
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "Actor Spawner Initialized");
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Actor Spawner Not Found");
	}
	
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
		OnStageStart.Broadcast(CurrentStage.SpawnActorKey);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "No stage found");
	}
}

void AUPStageManager::CompleteStage()
{
	FString str = FString::Printf(TEXT("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Stage %d Clear!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"), CurrentStageIndex);
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, *str);
	OnStageClear.Broadcast(CurrentStage.SpawnActorKey);

	CurrentStageIndex++;
	if (StageTutorialData->TutorialStages.IsValidIndex(CurrentStageIndex))
	{
		StartStage(CurrentStageIndex);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "!!!!!!!!!!!!!!!!!!!!!!!!!All Stage Clear!!!!!!!!!!!!!!!!!!!!!!");
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