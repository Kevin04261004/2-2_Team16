// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/UPActorSpawnerSubsystem.h"
#include "UPActorSpawnerSubsystem.h"

#include "Character/Enemy/UPMonsterBase.h"
#include "GameData/FUPSpawnActorData.h"
#include "UObject/ConstructorHelpers.h"

void UUPActorSpawnerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	// 데이터 테이블 경로
	const FString DataTablePath = TEXT("/Game/UniversityProject/GameData/Stage/DT_SpawnActor.DT_SpawnActor");

	// 런타임에 객체 로드
	UObject* DataTableObject = StaticLoadObject(UDataTable::StaticClass(), nullptr, *DataTablePath);
	if (DataTableObject)
	{
		ActorSpawnDataTable = Cast<UDataTable>(DataTableObject);
		if (ActorSpawnDataTable)
		{
			UE_LOG(LogTemp, Log, TEXT("ActorSpawnDataTable successfully loaded in Subsystem."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load ActorSpawnDataTable in Subsystem."));
	}
}

void UUPActorSpawnerSubsystem::Deinitialize()
{
	Super::Deinitialize();

	UE_LOG(LogTemp, Log, TEXT("UUPActorSpawnerSubsystem deinitialized."));
}

void UUPActorSpawnerSubsystem::SpawnActorsWhenStageStart(FString SpawnActorKey)
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

void UUPActorSpawnerSubsystem::SpawnActor(const FUPSpawnActorData& SpawnData)
{
	if (!SpawnData.ActorClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnData ActorClass is null!"));
		return;
	}

	// Actor를 스폰
	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(
		SpawnData.ActorClass,
		SpawnData.SpawnLocation,
		SpawnData.SpawnRotation
	);

	if (SpawnedActor)
	{
		// UPMonsterBase 여부 체크
		AUPMonsterBase* SpawnedMonster = Cast<AUPMonsterBase>(SpawnedActor);
		if (SpawnedMonster)
		{
			// UPMonsterBase일 경우에만 AIController 추가
			APawn* SpawnedPawn = Cast<APawn>(SpawnedMonster);
			if (SpawnedPawn && SpawnedPawn->Controller == nullptr)
			{
				if (SpawnedPawn->AIControllerClass != nullptr)
				{
					AUPPettuAIController* NewController = GetWorld()->SpawnActor<AUPPettuAIController>(SpawnedPawn->AIControllerClass);
					if (NewController)
					{
						NewController->Possess(SpawnedPawn);
					}
				}
			}
		}

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

	SpawnedActors.Add(SpawnedActor);
}

void UUPActorSpawnerSubsystem::ClearSpawnedActors()
{
	for (AActor* SpawnedActor : SpawnedActors)
	{
		if (SpawnedActor && !SpawnedActor->IsPendingKillPending())
		{
			SpawnedActor->Destroy(); // 액터 삭제
		}
	}

	SpawnedActors.Empty();

	UE_LOG(LogTemp, Log, TEXT("All spawned actors have been cleared."));
}

