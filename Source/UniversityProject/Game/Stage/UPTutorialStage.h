#pragma once

#include "CoreMinimal.h"
#include "UPTutorialStage.generated.h"

UENUM(BlueprintType)
enum class EStageConditionType : uint8
{
	UseBaseAttack UMETA(DisplayName = "UseBaseAttack"),
	UseDash UMETA(DisplayName = "UseDash"),
	ReachLocation UMETA(DisplayName = "ReachLocation"),
};

USTRUCT(BlueprintType)
struct FUPTutorialStage
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EStageConditionType, int> StageConditionMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsComplete;

	FUPTutorialStage() : bIsComplete(false)
	{
		StageConditionMap.Empty();
	}
};