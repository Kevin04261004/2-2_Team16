#pragma once

#include "CoreMinimal.h"
#include "UPTutorialStage.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnStageClear)
DECLARE_MULTICAST_DELEGATE(FOnStageConditionUpdate)
DECLARE_MULTICAST_DELEGATE(FOnStageStart)

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Tooltip = "DataTable에서 생성할 때 사용할 키 값"))
	FString SpawnActorKey;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EStageConditionType, int /* count */> StageConditionMap;

	FOnStageClear OnStageClear;
	FOnStageConditionUpdate OnStageConditionUpdate;
	FOnStageStart OnStageStart;
	
	FUPTutorialStage() = default;
};