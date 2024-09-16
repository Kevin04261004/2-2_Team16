// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/UPPettuAIController.h"
#include "Character/UPPettuCharacter.h"
#include "BehaviorTree/BehaviorTree.h"

AUPPettuAIController::AUPPettuAIController(FObjectInitializer const& ObjectInitializer)
{
}

void AUPPettuAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (AUPPettuCharacter* const Pettu = Cast<AUPPettuCharacter>(InPawn))
	{
		if (UBehaviorTree* const tree = Pettu->GetBehaviorTree())
		{
			UBlackboardComponent* b;
			UseBlackboard(tree->BlackboardAsset, b);
			Blackboard = b;
			RunBehaviorTree(tree);
		}
	}
}
