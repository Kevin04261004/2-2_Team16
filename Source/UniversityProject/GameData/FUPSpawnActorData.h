#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FUPSpawnActorData.generated.h"

USTRUCT(BlueprintType)
struct FUPSpawnActorData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SpawnActorKey;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector SpawnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator SpawnRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, float> ActorAttributes;
};