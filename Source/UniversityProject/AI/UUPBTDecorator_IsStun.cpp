// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/UUPBTDecorator_IsStun.h"
#include "UPPettuAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Enemy/UPPettuCharacter.h"

UUUPBTDecorator_IsStun::UUUPBTDecorator_IsStun(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("Stun Check");
}

bool UUUPBTDecorator_IsStun::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	auto* const PettuController = Cast<AUPPettuAIController>(OwnerComp.GetAIOwner());
	if (PettuController)
	{
		auto* const Pettu = Cast<AUPPettuCharacter>(PettuController->GetPawn());
		if (Pettu)
		{
			return OwnerComp.GetBlackboardComponent()->GetValueAsBool(TEXT("IsStun"));
		}
	}
	return false;
}
