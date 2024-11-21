// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/UUPBTDecorator_IsDead.h"
#include "UPPettuAIController.h"
#include "Character/UPCharacterBase.h"


UUUPBTDecorator_IsDead::UUUPBTDecorator_IsDead(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("Dead Check");
}

bool UUUPBTDecorator_IsDead::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	auto* const PettuController = Cast<AUPPettuAIController>(OwnerComp.GetAIOwner());
	if (PettuController)
	{
		auto* const EnemyCharacter = Cast<AUPCharacterBase>(PettuController->GetPawn());
		if (EnemyCharacter)
		{
			return EnemyCharacter->IsDead();
		}
	}
	return false;
}
