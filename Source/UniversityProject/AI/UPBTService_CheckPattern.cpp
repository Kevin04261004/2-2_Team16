// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/UPBTService_CheckPattern.h"
#include "BehaviorTree/BlackboardComponent.h"

UUPBTService_CheckPattern::UUPBTService_CheckPattern(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("Check Pattern");
}

void UUPBTService_CheckPattern::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	bIsStun = OwnerComp.GetBlackboardComponent()->GetValueAsBool(TEXT("IsStun"));
	bIsDead = OwnerComp.GetBlackboardComponent()->GetValueAsBool(TEXT("IsDead"));
	if (!bIsStun && !bIsDead)
	{
		ElapsedTime += DeltaSeconds;
		if (ElapsedTime >= TimerInterval)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("CanExecutePattern"), true);
			ResetTimer();
		}
	}
}

void UUPBTService_CheckPattern::ResetTimer()
{
	ElapsedTime = 0.0f;
}
