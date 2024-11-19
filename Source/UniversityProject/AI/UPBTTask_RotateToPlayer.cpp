// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/UPBTTask_RotateToPlayer.h"

#include "UPPettuAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/UPPlayerCharacter.h"
#include "Character/Enemy/UPMonsterBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

UUPBTTask_RotateToPlayer::UUPBTTask_RotateToPlayer(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("RotateToPlayer");
	bNotifyTick = true;
}

EBTNodeResult::Type UUPBTTask_RotateToPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AIController = Cast<AUPPettuAIController>(OwnerComp.GetAIOwner());
	if (AIController)
	{
		AIPawn = Cast<AUPMonsterBase>(AIController->GetPawn());
		if (AIPawn)
		{
			PlayerCharacter = Cast<AUPPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
			if (PlayerCharacter)
			{
				AUPPettuAIController* PettuAIController = Cast<AUPPettuAIController>(AIController);
				if (PettuAIController)
				{
					TargetRotation = UKismetMathLibrary::FindLookAtRotation(AIPawn->GetActorLocation(), PlayerCharacter->GetActorLocation());
					StartRotation = AIPawn->GetActorRotation();
					ElapsedTime = 0.0f;
					return EBTNodeResult::InProgress;
				}
			}
		}
	}
	return EBTNodeResult::Failed;
}

void UUPBTTask_RotateToPlayer::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	AIController = Cast<AUPPettuAIController>(OwnerComp.GetAIOwner());
	if (AIController)
	{
		AIPawn = Cast<AUPMonsterBase>(AIController->GetPawn());
		if (AIPawn)
		{
			// 지정한 시간 동안 목표 회전으로 보간
			ElapsedTime += DeltaSeconds;
			float Alpha = FMath::Clamp(ElapsedTime / RotationTime, 0.0f, 1.0f);

			FRotator NewRotation = FMath::Lerp(StartRotation, TargetRotation, Alpha);
			NewRotation.Pitch = StartRotation.Pitch;
			NewRotation.Roll = StartRotation.Roll;
			AIPawn->SetActorRotation(NewRotation);
			//GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Cyan, FString::Printf(TEXT("%f"), Alpha));

			// 목표 회전에 도달하면 태스크 완료
			if (Alpha >= 1.0f)
			{
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			}
		}
	}
}
