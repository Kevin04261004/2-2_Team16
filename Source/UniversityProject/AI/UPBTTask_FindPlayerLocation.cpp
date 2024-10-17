// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/UPBTTask_FindPlayerLocation.h"

#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

UUPBTTask_FindPlayerLocation::UUPBTTask_FindPlayerLocation(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("Find Player Location");
	PreLocation = FVector::ZeroVector;
}

EBTNodeResult::Type UUPBTTask_FindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// 플레이어 위치 따옴
	if (auto* const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		auto const PlayerLocation = Player->GetActorLocation();
		if (PlayerLocation != PreLocation)
		{
			PreLocation = PlayerLocation;
		}
		UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
		if (NavSys != nullptr)
		{
			FNavLocation NavLocation;
			bool bIsOnNavMesh = NavSys->ProjectPointToNavigation(PreLocation, NavLocation);
			// 플레이어가 네비 메시 바운더리 밖으로 나갔다면 특정 위치로 이동하도록 설정
			if (!bIsOnNavMesh)
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), FallbackLocation); // 지정된 위치로 이동
				OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("PlayerLost"), true); // 플레이어 감지 실패
			}
			else
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), PreLocation);
				OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("PlayerLost"), false); // 플레이어 감지 성공
			}
		}
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
