// Fill out your copyright notice in the Description page of Project Settings.


#include "State/UPPlayerIdleState.h"

#include "Character/UPPlayerCharacter.h"
#include "Components/UPInputHandlerComponent.h"

UUPPlayerIdleState::UUPPlayerIdleState()
{
}

void UUPPlayerIdleState::Initialize(AUPPlayerCharacter* InOwnerCharacter, class UUPInputHandlerComponent* InInputHandler)
{
	Super::Initialize(InOwnerCharacter, InInputHandler);
}

void UUPPlayerIdleState::EnterState()
{
	Super::EnterState();
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "Player Idle Enter");
}

void UUPPlayerIdleState::ExitState()
{
	Super::ExitState();
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Player Idle Exit");
}

void UUPPlayerIdleState::UpdateState()
{
	Super::UpdateState();

	if (InputHandler->IsMoving())
	{
		ChangeState(InputHandler->IsSprint() ? EPlayerStateType::Sprint : EPlayerStateType::Walk);
	}
}
