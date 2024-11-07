// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/UPActorSpawner.h"

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
		SpawnActor(*Row);
	}
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
