// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	UDataTable* ActorSpawnDataTable;

	UFUNCTION()
	void SpawnActors(FString SpawnActorKey);

private:
	void SpawnActor(const FUPSpawnActorData& SpawnData);
};
