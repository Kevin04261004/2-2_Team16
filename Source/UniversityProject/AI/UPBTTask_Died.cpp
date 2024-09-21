// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Enemy/UPPettuCharacter.h"
#include "AI/UPPettuAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/UPBTTask_Died.h"

UUPBTTask_Died::UUPBTTask_Died(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("Died");
}

EBTNodeResult::Type UUPBTTask_Died::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto* const PettuController = Cast<AUPPettuAIController>(OwnerComp.GetAIOwner());
	if (PettuController)
	{
		auto* const Pettu = Cast<AUPPettuCharacter>(PettuController->GetPawn());
		/*if (Pettu->IsDead())
		{
			//Pettu->Died();
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("IsDead"), true);
			return EBTNodeResult::Succeeded;
		}
		else
		{
			// 캐릭터가 사망하지 않은 경우
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("IsDead"), false);
			return EBTNodeResult::Failed;
		}*/

	}
	return EBTNodeResult::Failed;
}
