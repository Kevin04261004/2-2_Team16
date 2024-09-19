// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/UPBTTask_FindPlayerLocation.h"

#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

UUPBTTask_FindPlayerLocation::UUPBTTask_FindPlayerLocation(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("Find Player Location");
}

EBTNodeResult::Type UUPBTTask_FindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// 플레이어 위치 따옴
	if (auto* const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		auto const PlayerLocation = Player->GetActorLocation();
		FNavLocation Loc;
		if (auto* const Naysys = UNavigationSystemV1::GetCurrent(GetWorld()))
		{
			if (Naysys->GetRandomPointInNavigableRadius(PlayerLocation, SearchRadius, Loc))
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), Loc.Location);
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}
