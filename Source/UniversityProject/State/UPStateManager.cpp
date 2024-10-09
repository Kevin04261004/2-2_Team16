// Fill out your copyright notice in the Description page of Project Settings.

#include "UPStateManager.h"

#include "UPPlayerBaseAttack01State.h"
#include "UPPlayerBaseAttack02State.h"
#include "UPPlayerBaseAttack03State.h"
#include "UPPlayerIdleState.h"
#include "UPPlayerSprintState.h"
#include "UPPlayerWalkState.h"
#include "UPPlayerBaseState.h"
#include "UPPlayerDashState.h"
#include "UPPlayerJumpState.h"
#include "UPPlayerTakeDownState.h"
#include "UPPlayerUpperCutState.h"
#include "Character/UPPlayerCharacter.h"

UUPStateManager::UUPStateManager()
{
	// InitializeStateMap();
}

void UUPStateManager::Initialize(UUPInputHandlerComponent* InInputHandler)
{
	OwningCharacter = Cast<AUPPlayerCharacter>(GetOwner());
	this->InputHandler = InInputHandler;
}

void UUPStateManager::InitializeStateMap()
{
	// TODO: 코드 최적화 엔진에서 추가 가능하게
	StateMap.Add(EPlayerStateType::Idle, NewObject<UUPPlayerIdleState>());
	StateMap.Add(EPlayerStateType::Walk, NewObject<UUPPlayerWalkState>());
	StateMap.Add(EPlayerStateType::Sprint, NewObject<UUPPlayerSprintState>());
	StateMap.Add(EPlayerStateType::Jump, NewObject<UUPPlayerJumpState>());
	StateMap.Add(EPlayerStateType::Dash, NewObject<UUPPlayerDashState>());
	StateMap.Add(EPlayerStateType::BaseAttack01, NewObject<UUPPlayerBaseAttack01State>());
	StateMap.Add(EPlayerStateType::BaseAttack02, NewObject<UUPPlayerBaseAttack02State>());
	StateMap.Add(EPlayerStateType::BaseAttack03, NewObject<UUPPlayerBaseAttack03State>());
	StateMap.Add(EPlayerStateType::TakeDown, NewObject<UUPPlayerTakeDownState>());
	StateMap.Add(EPlayerStateType::UpperCut, NewObject<UUPPlayerUpperCutState>());
}

void UUPStateManager::InitializeStates(const EPlayerStateType InitState)
{
	StateMap[EPlayerStateType::Idle]->Initialize(OwningCharacter, InputHandler);
	StateMap[EPlayerStateType::Walk]->Initialize(OwningCharacter, InputHandler);
	StateMap[EPlayerStateType::Sprint]->Initialize(OwningCharacter, InputHandler);
	StateMap[EPlayerStateType::Jump]->Initialize(OwningCharacter, InputHandler);
	StateMap[EPlayerStateType::Dash]->Initialize(OwningCharacter, InputHandler);
	StateMap[EPlayerStateType::BaseAttack01]->Initialize(OwningCharacter, InputHandler);
	StateMap[EPlayerStateType::BaseAttack02]->Initialize(OwningCharacter, InputHandler);
	StateMap[EPlayerStateType::BaseAttack03]->Initialize(OwningCharacter, InputHandler);
	StateMap[EPlayerStateType::TakeDown]->Initialize(OwningCharacter, InputHandler);
	StateMap[EPlayerStateType::UpperCut]->Initialize(OwningCharacter, InputHandler);
	
	CurrentStateType = InitState;
	StateMap[CurrentStateType]->EnterState();
}

void UUPStateManager::UpdateState()
{
	if (StateMap.Find(CurrentStateType) && StateMap[CurrentStateType] != nullptr)
	{
		IsValid(StateMap[CurrentStateType]);
		StateMap[CurrentStateType]->UpdateState();
	}
}

void UUPStateManager::ChangeState(EPlayerStateType NextState)
{
	if (!StateMap.Find(NextState))
	{
		return;
	}
	if (StateMap.Find(CurrentStateType) && StateMap[CurrentStateType] != nullptr)
	{
		StateMap[CurrentStateType]->ExitState();
	}
	CurrentStateType = NextState;
	StateMap[CurrentStateType]->EnterState();
} // 좀 멋지네요 (by. 지나가는 똥)
