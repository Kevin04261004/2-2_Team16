// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "UUPBTDecorator_IsStun.generated.h"

/**
 * 
 */
UCLASS()
class UNIVERSITYPROJECT_API UUUPBTDecorator_IsStun : public UBTDecorator
{
	GENERATED_BODY()
public:
	UUUPBTDecorator_IsStun(const FObjectInitializer& ObjectInitializer);
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
