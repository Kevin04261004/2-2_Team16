// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/UPBTTask_ChasePlayer.h"

#include "UPPettuAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Navigation/PathFollowingComponent.h"

UUPBTTask_ChasePlayer::UUPBTTask_ChasePlayer(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("ChasePlayer");
}

EBTNodeResult::Type UUPBTTask_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto* const cont = Cast<AUPPettuAIController>(OwnerComp.GetAIOwner()))
	{
		auto const TargetLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey());
		//UAIBlueprintHelperLibrary::SimpleMoveToLocation(cont, TargetLocation);
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalLocation(TargetLocation);
		MoveRequest.SetAcceptanceRadius(5.0f);

		FNavPathSharedPtr NavPath;

		cont->MoveTo(MoveRequest, &NavPath);

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
