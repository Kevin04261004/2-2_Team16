// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "UPBTTask_PlayMontage.generated.h"

/**
 * 
 */
UCLASS()
class UNIVERSITYPROJECT_API UUPBTTask_PlayMontage : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UUPBTTask_PlayMontage(const FObjectInitializer& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UFUNCTION()
	void MontageEnd(UAnimMontage* Montage, bool bInterrupted);

	UPROPERTY(EditAnywhere)
	UAnimMontage* AnimMontage;

private:
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> CurrentOwnerComp;
};
