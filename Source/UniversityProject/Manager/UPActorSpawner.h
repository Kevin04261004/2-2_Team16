// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UPActorSpawnerSubsystem.h"
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

	UPROPERTY()
	TObjectPtr<UUPActorSpawnerSubsystem> ActorSpawnerSubsystem;
	
	UUPActorSpawner();
	
	UFUNCTION()
	void SpawnActorsWhenStageStart(FString SpawnActorKey) const;
	void ClearSpawnedActors();

	void InitializeSpawner(AUPStageManager* StageManager);


private:
	void SpawnBoss();
};
