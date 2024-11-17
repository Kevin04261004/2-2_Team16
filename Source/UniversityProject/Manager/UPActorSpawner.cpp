// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/UPActorSpawner.h"

UUPActorSpawner::UUPActorSpawner()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableFinder(TEXT("/Game/UniversityProject/GameData/Stage/DT_SpawnActor.DT_SpawnActor"));
	if (DataTableFinder.Succeeded())
	{
		ActorSpawnDataTable = DataTableFinder.Object;
		UE_LOG(LogTemp, Log, TEXT("ActorSpawnDataTable successfully loaded in UUPActorSpawner constructor."));
	}
}

void UUPActorSpawner::SpawnActors(FString SpawnActorKey)
{
	if (!ActorSpawnDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("ActorSpawnDataTable is not set!"));
		return;
	}

	FString ContextString;
	TArray<FUPSpawnActorData*> Rows;
	ActorSpawnDataTable->GetAllRows(ContextString, Rows);

	for (FUPSpawnActorData* Row : Rows)
	{
		if (Row->SpawnActorKey == SpawnActorKey)
		{
			SpawnActor(*Row);
		}
	}
}

void UUPActorSpawner::InitializeSpawner(AUPStageManager* StageManager)
{
	if (!StageManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("StageManager is null!"));
		return;
	}

	// Bind the OnStageStart event to SpawnActors
	StageManager->OnStageStart.AddUObject(this, &UUPActorSpawner::SpawnActors);

	UE_LOG(LogTemp, Log, TEXT("ActorSpawner successfully bound to StageManager"));
}

void UUPActorSpawner::SpawnActor(const FUPSpawnActorData& SpawnData)
{
	if (!SpawnData.ActorClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnData ActorClass is null!"));
		return;
	}

	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(
		SpawnData.ActorClass,
		SpawnData.SpawnLocation,
		SpawnData.SpawnRotation
	);

	if (SpawnedActor)
	{
		// ActorAttributes에 정의된 속성값을 Actor에 적용
		for (const TPair<FString, float>& Attribute : SpawnData.ActorAttributes)
		{
			FName PropertyName = FName(*Attribute.Key);
			FProperty* Property = SpawnedActor->GetClass()->FindPropertyByName(PropertyName);
            
			if (Property)
			{
				void* PropertyValue = Property->ContainerPtrToValuePtr<void>(SpawnedActor);
				if (FFloatProperty* FloatProperty = CastField<FFloatProperty>(Property))
				{
					FloatProperty->SetPropertyValue(PropertyValue, Attribute.Value);
				}
				// 다른 속성 유형 추가 가능
			}
		}
	}
}
