// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/UPActorSpawner.h"

#include "Character/Enemy/UPMonsterBase.h"

UUPActorSpawner::UUPActorSpawner()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableFinder(TEXT("/Game/UniversityProject/GameData/Stage/DT_SpawnActor.DT_SpawnActor"));
	if (DataTableFinder.Succeeded())
	{
		ActorSpawnDataTable = DataTableFinder.Object;
		UE_LOG(LogTemp, Log, TEXT("ActorSpawnDataTable successfully loaded in UUPActorSpawner constructor."));
	}
}

void UUPActorSpawner::SpawnActorsWhenStageStart(FString SpawnActorKey)
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

	StageManager->OnStageStart.AddUObject(this, &UUPActorSpawner::SpawnActorsWhenStageStart);
	
	UE_LOG(LogTemp, Log, TEXT("ActorSpawner successfully bound to StageManager"));
}

void UUPActorSpawner::SpawnActor(const FUPSpawnActorData& SpawnData)
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
}
