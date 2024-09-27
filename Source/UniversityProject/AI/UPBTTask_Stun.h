// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Character/Enemy/UPPettuCharacter.h"
#include "UPBTTask_Stun.generated.h"

/**
 * 
 */
UCLASS()
class UNIVERSITYPROJECT_API UUPBTTask_Stun : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UUPBTTask_Stun(const FObjectInitializer& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	UFUNCTION()
	void OnStunEnded(UAnimMontage* Montage, bool bInterrupted);
private:
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> CurrentOwnerComp;

	TObjectPtr<AUPPettuCharacter> PettuCharacter;

	UPROPERTY()
	TObjectPtr<UAnimInstance> AnimInstance;
};
