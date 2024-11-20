// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/UPBTTask_SetPatternTimer.h"

#include "BehaviorTree/BlackboardComponent.h"

UUPBTTask_SetPatternTimer::UUPBTTask_SetPatternTimer(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("Set Pattern Timer");
}

EBTNodeResult::Type UUPBTTask_SetPatternTimer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), bIsTimerOn);
	return EBTNodeResult::Succeeded;
}
