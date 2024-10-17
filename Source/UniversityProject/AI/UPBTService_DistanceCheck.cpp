// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/UPBTService_DistanceCheck.h"

#include "UPPettuAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/UPPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

UUPBTService_DistanceCheck::UUPBTService_DistanceCheck(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("DistanceCheck");
	bNotifyBecomeRelevant = true;
	Distance = 50.0f;
}

void UUPBTService_DistanceCheck::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AUPPettuAIController* const cont = Cast<AUPPettuAIController>(OwnerComp.GetAIOwner());
	if (!cont)
	{
		return;
	}

	APawn* AIPawn = cont->GetPawn();
	if (!AIPawn)
	{
		return;
	}

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return;
	}

	AUPPlayerCharacter* Player = Cast<AUPPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!Player)
	{
		return;
	}

	float DistanceToPlayer = FVector::Dist(AIPawn->GetActorLocation(), Player->GetActorLocation());
	bool bIsInRange = DistanceToPlayer <= Distance;
	BlackboardComp->SetValueAsBool("IsInRange", bIsInRange);
}
