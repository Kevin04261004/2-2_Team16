// Fill out your copyright notice in the Description page of Project Settings.

#include "UPStateManager.h"

#include "UPPlayerIdleState.h"
#include "UPPlayerRunState.h"
#include "UPPlayerWalkState.h"
#include "UPPlayerBaseState.h"
#include "Character/UPPlayerCharacter.h"

UUPStateManager::UUPStateManager()
{
	InitializeStateMap();
}

void UUPStateManager::Initialize(UUPInputHandlerComponent* InInputHandler)
{
	OwningCharacter = Cast<AUPPlayerCharacter>(GetOwner());
	this->InputHandler = InInputHandler;
}

void UUPStateManager::InitializeStateMap()
{
	StateMap.Add(EPlayerStateType::Idle, NewObject<UUPPlayerIdleState>());
	StateMap.Add(EPlayerStateType::Walk, NewObject<UUPPlayerWalkState>());
	StateMap.Add(EPlayerStateType::Sprint, NewObject<UUPPlayerRunState>());
}

void UUPStateManager::InitializeStates(const EPlayerStateType InitState)
{
	StateMap[EPlayerStateType::Idle]->Initialize(OwningCharacter, InputHandler);
	StateMap[EPlayerStateType::Walk]->Initialize(OwningCharacter, InputHandler);
	StateMap[EPlayerStateType::Sprint]->Initialize(OwningCharacter, InputHandler);


	CurrentState = InitState;
	StateMap[CurrentState]->EnterState();
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
