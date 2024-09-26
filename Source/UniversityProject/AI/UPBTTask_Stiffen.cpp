// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/UPBTTask_Stiffen.h"

#include "UPPettuAIController.h"
#include "Character/Enemy/UPPettuCharacter.h"

UUPBTTask_Stiffen::UUPBTTask_Stiffen(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("Stiffen");
}

EBTNodeResult::Type UUPBTTask_Stiffen::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto* const PettuController = Cast<AUPPettuAIController>(OwnerComp.GetAIOwner());
	auto* const Pettu = Cast<AUPPettuCharacter>(PettuController->GetPawn());
	if (Pettu)
	{
		CurrentOwnerComp = &OwnerComp;

		AnimInstance = Pettu->GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->OnMontageEnded.AddDynamic(this, &UUPBTTask_Stiffen::OnStiffenEnded);
		}
		return EBTNodeResult::InProgress; 
	}
	return EBTNodeResult::Failed;
}

void UUPBTTask_Stiffen::OnStiffenEnded(UAnimMontage* Montage, bool bInterrupted)
{
	AnimInstance->OnMontageEnded.RemoveDynamic(this, &UUPBTTask_Stiffen::OnStiffenEnded);
	FinishLatentTask(*CurrentOwnerComp, EBTNodeResult::Succeeded);
}
