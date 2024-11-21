// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Character/Enemy/UPMonsterBase.h"
#include "UPBTTask_Pattern1.generated.h"

/**
 * 
 */
UCLASS()
class UNIVERSITYPROJECT_API UUPBTTask_Pattern1 : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UUPBTTask_Pattern1(const FObjectInitializer& ObjectInitializer);

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
private:
	UFUNCTION()
	void OnPatternMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	TObjectPtr<UBehaviorTreeComponent> CurrentOwnerComp;

	UPROPERTY()
	TObjectPtr<UAnimInstance> AnimInstance;

	UPROPERTY(EditAnywhere, Category = "Pattern")
	EPettuSkillType SkillType;

	UPROPERTY()
	TObjectPtr<AUPMonsterBase> MonsterCharacter;
	
};
