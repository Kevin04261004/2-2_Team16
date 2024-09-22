// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/UPBTTask_Stun.h"
#include "UPPettuAIController.h"
#include "Character/Enemy/UPPettuCharacter.h"

UUPBTTask_Stun::UUPBTTask_Stun(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("Stun");
}

EBTNodeResult::Type UUPBTTask_Stun::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto* const PettuController = Cast<AUPPettuAIController>(OwnerComp.GetAIOwner());
	auto* const Pettu = Cast<AUPPettuCharacter>(PettuController->GetPawn());
	if (Pettu)
	{
		// 스턴 애니메이션 재생
		Pettu->SetPettuStun();

		CurrentOwnerComp = &OwnerComp;
		
		UAnimInstance* AnimInstance = Pettu->GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->OnMontageEnded.AddDynamic(this, &UUPBTTask_Stun::OnStunEnded);
		}

		return EBTNodeResult::InProgress;  // 애니메이션 끝날 때까지 대기
	}

	return EBTNodeResult::Failed;
}

void UUPBTTask_Stun::OnStunEnded(UAnimMontage* Montage, bool bInterrupted)
{
	FinishLatentTask(*CurrentOwnerComp, EBTNodeResult::Succeeded);
}
