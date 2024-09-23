// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "UPBTService_CheckPattern.generated.h"

/**
 * 
 */
UCLASS()
class UNIVERSITYPROJECT_API UUPBTService_CheckPattern : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	UUPBTService_CheckPattern(const FObjectInitializer& ObjectInitializer);
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
	float TimerInterval = 5.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Timer")
	float ElapsedTime = 0.0f;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	void ResetTimer();
};
