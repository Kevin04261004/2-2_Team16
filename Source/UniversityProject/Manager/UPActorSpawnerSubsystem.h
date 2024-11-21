// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameData/FUPSpawnActorData.h"
#include "Subsystems/WorldSubsystem.h"
#include "UPActorSpawnerSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class UNIVERSITYPROJECT_API UUPActorSpawnerSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable)
	void SpawnActorsWhenStageStart(FString SpawnActorKey);
	void SpawnActor(const FUPSpawnActorData& SpawnData);

	void ClearSpawnedActors();
private:
	UPROPERTY()
	UDataTable* ActorSpawnDataTable;

	TArray<AActor*> SpawnedActors;
};
