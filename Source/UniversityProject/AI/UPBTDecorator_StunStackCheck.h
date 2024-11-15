// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "UPBTDecorator_StunStackCheck.generated.h"

/**
 * 
 */
UCLASS()
class UNIVERSITYPROJECT_API UUPBTDecorator_StunStackCheck : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
public:
	UUPBTDecorator_StunStackCheck(const FObjectInitializer& ObjectInitializer);
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	UPROPERTY(EditAnywhere)
	float InputStunStack = 0.0f;

	
};
