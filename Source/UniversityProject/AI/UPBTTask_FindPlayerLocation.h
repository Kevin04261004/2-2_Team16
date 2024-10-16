// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "UPBTTask_FindPlayerLocation.generated.h"

/**
 * 
 */
UCLASS()
class UNIVERSITYPROJECT_API UUPBTTask_FindPlayerLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	explicit UUPBTTask_FindPlayerLocation(const FObjectInitializer& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
private:
	// 플레이어 위치 감지반경 나중에 수정할 것
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Search", meta = (AllowPrivateAccess = "true"))
	float SearchRadius = 150.0f;

	FVector PreLocation;
};
