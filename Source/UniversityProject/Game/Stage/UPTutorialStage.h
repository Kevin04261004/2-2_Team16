#pragma once

UENUM(BlueprintType)
enum class EStageConditionType : uint8
{
	UseDash UMETA(DisplayName = "UseDash"),
	ReachLocation UMETA(DisplayName = "ReachLocation"),
};

USTRUCT(BlueprintType)
struct FUPTutorialStage
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EStageConditionType ConditionType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsComplete = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int count;

	// 필요에 따라 조건 평가에 사용할 추가 데이터
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector TargetLocation;

	FUPTutorialStage()
	{
		// ConditionType = EStageConditionType::
	}
};