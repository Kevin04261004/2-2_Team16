// Fill out your copyright notice in the Description page of Project Settings.

#include "UPPlayerBaseState.h"
#include "UPStateManager.h"

#include "UPPlayerIdleState.h"
#include "UPPlayerRunState.h"
#include "UPPlayerWalkState.h"
#include "Character/UPPlayerCharacter.h"

UUPStateManager::UUPStateManager()
{
	InitializeStateMap();
}

void UUPStateManager::Initialize(EPlayerStateType InitState)
{
	OwningCharacter = Cast<AUPPlayerCharacter>(GetOwner());
	InitializeState();
	
	CurrentState = InitState;
	StateMap[CurrentState]->EnterState();
}

void UUPStateManager::InitializeStateMap()
{
	StateMap.Add(EPlayerStateType::Idle, NewObject<UUPPlayerIdleState>());
	StateMap.Add(EPlayerStateType::Walk, NewObject<UUPPlayerWalkState>());
	StateMap.Add(EPlayerStateType::Run, NewObject<UUPPlayerRunState>());
}

void UUPStateManager::InitializeState()
{
	StateMap[EPlayerStateType::Idle]->Initialize(OwningCharacter);
	StateMap[EPlayerStateType::Walk]->Initialize(OwningCharacter);
	StateMap[EPlayerStateType::Run]->Initialize(OwningCharacter);
}

void UUPStateManager::UpdateState()
{
	StateMap[CurrentState]->UpdateState();
}

void UUPStateManager::ChangeState(EPlayerStateType NextState)
{
	StateMap[CurrentState]->ExitState();
	CurrentState = NextState;
	StateMap[CurrentState]->EnterState();
} // 좀 멋지네요 (by. 지나가는 똥)
