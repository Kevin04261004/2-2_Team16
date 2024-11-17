// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/Stage/UPStageManager.h"
#include "GameData/FUPSpawnActorData.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UPActorSpawner.generated.h"

/**
 * 
 */
UCLASS()
class UNIVERSITYPROJECT_API UUPActorSpawner : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<UDataTable> ActorSpawnDataTable;

	UUPActorSpawner();
	
	UFUNCTION()
	void SpawnActorsWhenStageStart(FString SpawnActorKey);
	
	void InitializeSpawner(AUPStageManager* StageManager);
	
private:
	void SpawnActor(const FUPSpawnActorData& SpawnData);
};
