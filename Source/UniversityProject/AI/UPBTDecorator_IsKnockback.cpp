// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/UPBTDecorator_IsKnockback.h"
#include "UPPettuAIController.h"
#include "Character/Enemy/UPEnemyMonster.h"

UUPBTDecorator_IsKnockback::UUPBTDecorator_IsKnockback(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("Knockback Check");
}

bool UUPBTDecorator_IsKnockback::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	auto* const PettuController = Cast<AUPPettuAIController>(OwnerComp.GetAIOwner());
	if (PettuController)
	{
		auto* const EnemyCharacter = Cast<AUPEnemyMonster>(PettuController->GetPawn());
		if (EnemyCharacter)
		{
			return EnemyCharacter->IsKnockbackActive();
		}
	}
	return false;
}
