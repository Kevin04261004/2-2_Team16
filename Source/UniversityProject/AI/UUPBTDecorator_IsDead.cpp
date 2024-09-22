// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/UUPBTDecorator_IsDead.h"
#include "UPPettuAIController.h"
#include "Character/Enemy/UPPettuCharacter.h"


UUUPBTDecorator_IsDead::UUUPBTDecorator_IsDead(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("Dead Check");
}

bool UUUPBTDecorator_IsDead::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	auto* const PettuController = Cast<AUPPettuAIController>(OwnerComp.GetAIOwner());
	if (PettuController)
	{
		auto* const Pettu = Cast<AUPPettuCharacter>(PettuController->GetPawn());
		if (Pettu)
		{
			return Pettu->IsPettuDead();
		}
	}
	return false;
}
