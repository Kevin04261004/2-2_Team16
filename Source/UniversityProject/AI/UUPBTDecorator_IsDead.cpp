// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/UUPBTDecorator_IsDead.h"
#include "UPPettuAIController.h"
#include "Character/Enemy/UPMonsterBase.h"


UUUPBTDecorator_IsDead::UUUPBTDecorator_IsDead(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("Dead Check");
}

bool UUUPBTDecorator_IsDead::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	auto* const PettuController = Cast<AUPPettuAIController>(OwnerComp.GetAIOwner());
	if (PettuController)
	{
		auto* const EnemyCharacter = Cast<AUPMonsterBase>(PettuController->GetPawn());
		if (EnemyCharacter)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("IsDead : %d"), EnemyCharacter->IsDead()));
			return EnemyCharacter->IsDead();
		}
	}
	return false;
}
