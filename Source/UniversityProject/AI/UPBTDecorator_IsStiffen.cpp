// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/UPBTDecorator_IsStiffen.h"
#include "UPPettuAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Enemy/UPPettuCharacter.h"

UUPBTDecorator_IsStiffen::UUPBTDecorator_IsStiffen(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("Stiffen Check");
}

bool UUPBTDecorator_IsStiffen::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	auto* const PettuController = Cast<AUPPettuAIController>(OwnerComp.GetAIOwner());
	if (PettuController)
	{
		auto* const Pettu = Cast<AUPPettuCharacter>(PettuController->GetPawn());
		if (Pettu)
		{
			return OwnerComp.GetBlackboardComponent()->GetValueAsBool(TEXT("IsStiffen"));
		}
	}
	return false;
}
