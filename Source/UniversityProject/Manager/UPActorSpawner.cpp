// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/UPActorSpawner.h"

#include "UPActorSpawnerSubsystem.h"
#include "Character/Enemy/UPMonsterBase.h"

UUPActorSpawner::UUPActorSpawner()
{
}

void UUPActorSpawner::SpawnActorsWhenStageStart(FString SpawnActorKey) const
{
	ActorSpawnerSubsystem->SpawnActorsWhenStageStart(SpawnActorKey);
}

void UUPActorSpawner::InitializeSpawner(AUPStageManager* StageManager)
{
	ActorSpawnerSubsystem = GetWorld()->GetSubsystem<UUPActorSpawnerSubsystem>();
	if (!ActorSpawnerSubsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to retrieve ActorSpawnerSubsystem!"));
		return;
	}
	
	if (!StageManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("StageManager is null!"));
		return;
	}

	StageManager->OnStageStart.AddUObject(this, &UUPActorSpawner::SpawnActorsWhenStageStart);
	
	UE_LOG(LogTemp, Log, TEXT("ActorSpawner successfully bound to StageManager"));
}