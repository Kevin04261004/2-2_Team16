#pragma once

#include "CoreMinimal.h"
#include "UPTutorialStage.generated.h"

UENUM(BlueprintType)
enum class EStageConditionType : uint8
{
	None = 0,
	UseBaseAttack01 UMETA(DisplayName = "UseBaseAttack01"),
	UseBaseAttack02 UMETA(DisplayName = "UseBaseAttack02"),
	UseBaseAttack03 UMETA(DisplayName = "UseBaseAttack03"),
	UseDashSkill UMETA(DisplayName = "UseDashSkill"),
	UseJump UMETA(DisplayName = "UseJump"),
	UseTakeDown UMETA(DisplayName = "UseTakeDown"),
	UseUpperCut UMETA(DisplayName = "UseUpperCut"),
	FinishUseBaseAttack01 UMETA(DisplayName = "Finish UseBaseAttack01"),
	FinishUseBaseAttack02 UMETA(DisplayName = "Finish UseBaseAttack02"),
	FinishUseBaseAttack03 UMETA(DisplayName = "Finish UseBaseAttack03"),
	FinishUseDashSkill UMETA(DisplayName = "Finish UseDashSkill"),
	FinishUseJump UMETA(DisplayName = "Finish UseJump"),
	FinishUseTakeDown UMETA(DisplayName = "Finish UseTakeDown"),
	FinishUseUpperCut UMETA(DisplayName = "Finish UseUpperCut"),
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
	
	FUPTutorialStage() = default;
};