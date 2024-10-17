// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/UPBTTask_Pattern1.h"
#include "UPPettuAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Enemy/UPPettuCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Physics/Collision.h"

UUPBTTask_Pattern1::UUPBTTask_Pattern1(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("Pattern1");
}

EBTNodeResult::Type UUPBTTask_Pattern1::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	CurrentOwnerComp = &OwnerComp;
	auto* const PettuController = Cast<AUPPettuAIController>(OwnerComp.GetAIOwner());
	PettuCharacter = Cast<AUPPettuCharacter>(PettuController->GetPawn());
	AnimInstance = PettuCharacter->GetMesh()->GetAnimInstance();
	if (PettuCharacter)
	{
		PettuCharacter->SkillAttack(SkillType);
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(PettuCharacter->GetActorLocation(),
			UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation());
		FRotator NewRotation = FRotator(0.0f, LookAtRotation.Yaw, 0.0f);
		PettuCharacter->SetActorRotation(NewRotation);
		AnimInstance->OnMontageEnded.AddDynamic(this, &UUPBTTask_Pattern1::OnPatternMontageEnded);
		return EBTNodeResult::InProgress;
	}
	return EBTNodeResult::Failed;
}

void UUPBTTask_Pattern1::OnPatternMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	AnimInstance->OnMontageEnded.RemoveDynamic(this, &UUPBTTask_Pattern1::OnPatternMontageEnded);
	CurrentOwnerComp->GetBlackboardComponent()->SetValueAsBool(TEXT("CanExecutePattern"), false);
	FinishLatentTask(*CurrentOwnerComp, EBTNodeResult::Succeeded);
}
