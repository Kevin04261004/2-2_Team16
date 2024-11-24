// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/UPBTTask_PlayMontage.h"

#include "UPPettuAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Enemy/UPPettuCharacter.h"

UUPBTTask_PlayMontage::UUPBTTask_PlayMontage(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("PlayMontage");
}

EBTNodeResult::Type UUPBTTask_PlayMontage::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto* const PettuController = Cast<AUPPettuAIController>(OwnerComp.GetAIOwner());
	auto* const Pettu = Cast<AUPPettuCharacter>(PettuController->GetPawn());
	CurrentOwnerComp = &OwnerComp;
	if (Pettu)
	{
		UAnimInstance* AnimInstance = Pettu->GetMesh()->GetAnimInstance();
		if (AnimInstance == nullptr)
		{
			return EBTNodeResult::Failed;
		}
		AnimInstance->StopAllMontages(0.0f);
		if (AnimMontage)
		{
			AnimInstance->Montage_Play(AnimMontage, 1.0f);
			Pettu->GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &UUPBTTask_PlayMontage::MontageEnd);
		}
		return EBTNodeResult::InProgress;  // 애니메이션 끝날 때까지 대기
	}
	return EBTNodeResult::Failed;
}

void UUPBTTask_PlayMontage::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (OwnerComp.GetBlackboardComponent()->GetValueAsBool("IsStun"))
	{
		auto* const PettuController = Cast<AUPPettuAIController>(OwnerComp.GetAIOwner());
		auto* const Pettu = Cast<AUPPettuCharacter>(PettuController->GetPawn());
		if (Pettu && AnimMontage)
		{
			UAnimInstance* AnimInstance = Pettu->GetMesh()->GetAnimInstance();
			if (AnimInstance && AnimInstance->Montage_IsPlaying(AnimMontage))
			{
				AnimInstance->Montage_Stop(0.0f, AnimMontage);
			}
		}
		FinishLatentTask(OwnerComp, EBTNodeResult::Aborted);
	}
}

void UUPBTTask_PlayMontage::MontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	if (AnimMontage)
	{
		FinishLatentTask(*CurrentOwnerComp, EBTNodeResult::Succeeded);
	}
}
