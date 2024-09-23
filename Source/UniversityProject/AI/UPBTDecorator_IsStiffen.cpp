// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/UPBTDecorator_IsStiffen.h"

UUPBTDecorator_IsStiffen::UUPBTDecorator_IsStiffen(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("Stiffen Check");
}

bool UUPBTDecorator_IsStiffen::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	return Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
}
