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
		if (Pettu)
		{
			// 죽음 처리
			Pettu->SetPettuDead();
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("IsDead"), true);
			//FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded); 완료 안해도 될듯?
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
