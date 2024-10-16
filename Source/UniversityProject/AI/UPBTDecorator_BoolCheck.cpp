// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/UPBTDecorator_BoolCheck.h"

#include "BehaviorTree/BlackboardComponent.h"

bool UUPBTDecorator_BoolCheck::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	return OwnerComp.GetBlackboardComponent()->GetValueAsBool(GetSelectedBlackboardKey());
}
