// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "UPBTTask_Stiffen.generated.h"

/**
 * 
 */
UCLASS()
class UNIVERSITYPROJECT_API UUPBTTask_Stiffen : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UUPBTTask_Stiffen(const FObjectInitializer& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UFUNCTION()
	void OnStiffenEnded(UAnimMontage* Montage, bool bInterrupted);
	
private:
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> CurrentOwnerComp;

	UPROPERTY()
	TObjectPtr<UAnimInstance> AnimInstance;
};
