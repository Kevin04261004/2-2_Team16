// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/UPBTTask_Pattern1.h"
#include "UPPettuAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Enemy/UPPettuCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

UUPBTTask_Pattern1::UUPBTTask_Pattern1(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("Pattern1");
}

EBTNodeResult::Type UUPBTTask_Pattern1::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	CurrentOwnerComp = &OwnerComp;
	auto* const PettuController = Cast<AUPPettuAIController>(OwnerComp.GetAIOwner());
	auto* const Pettu = Cast<AUPPettuCharacter>(PettuController->GetPawn());
	AnimInstance = Pettu->GetMesh()->GetAnimInstance();
	if (Pettu)
	{
		Pettu->SkillAttack(SkillType);
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(Pettu->GetActorLocation(),
			UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation());
		FRotator NewRotation = FRotator(0.0f, LookAtRotation.Yaw, 0.0f);
		Pettu->SetActorRotation(NewRotation);
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("Pattern1"));
		AnimInstance->OnMontageEnded.AddDynamic(this, &UUPBTTask_Pattern1::OnPatternMontageEnded);
		return EBTNodeResult::InProgress;
	}
	return EBTNodeResult::Failed;
}

void UUPBTTask_Pattern1::OnPatternMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	AnimInstance->OnMontageEnded.RemoveDynamic(this, &UUPBTTask_Pattern1::OnPatternMontageEnded);
	CurrentOwnerComp->GetBlackboardComponent()->SetValueAsBool(TEXT("CanExecutePattern"), false);
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Pattern1 End")));
	FinishLatentTask(*CurrentOwnerComp, EBTNodeResult::Succeeded);
}
