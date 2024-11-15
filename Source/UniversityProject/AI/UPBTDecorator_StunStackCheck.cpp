// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/UPBTDecorator_StunStackCheck.h"

#include "UPPettuAIController.h"
#include "Character/Enemy/UPPettuCharacter.h"

UUPBTDecorator_StunStackCheck::UUPBTDecorator_StunStackCheck(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("Stun Stack Check");
}

bool UUPBTDecorator_StunStackCheck::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
                                                               uint8* NodeMemory) const
{
	auto* const PettuController = Cast<AUPPettuAIController>(OwnerComp.GetAIOwner());
	if (PettuController)
	{
		auto* const Pettu = Cast<AUPPettuCharacter>(PettuController->GetPawn());
		if (Pettu)
		{
			if (InputStunStack >= Pettu->GetStunStack())
			{
				return true;
			}
			else
				return false;
		}
	}
	return false;
}
