// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "UPBTTask_RotateToPlayer.generated.h"

/**
 * 
 */
UCLASS()
class UNIVERSITYPROJECT_API UUPBTTask_RotateToPlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UUPBTTask_RotateToPlayer(const FObjectInitializer& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere)
	float RotationTime = 0.0f;

private:
	UPROPERTY()
	class AUPPlayerCharacter* PlayerCharacter;

	UPROPERTY()
	class AUPPettuAIController* AIController;

	UPROPERTY()
	class AUPPettuCharacter* AIPawn;

	UPROPERTY()
	FRotator TargetRotation;
	UPROPERTY()
	FRotator StartRotation;
	UPROPERTY()
	float ElapsedTime;
};
