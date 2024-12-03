// Fill out your copyright notice in the Description page of Project Settings.


#include "State/UPPlayerLandState.h"

UUPPlayerLandState::UUPPlayerLandState()
{
}

void UUPPlayerLandState::Initialize(AUPPlayerCharacter* InOwnerCharacter,
	class UUPInputHandlerComponent* InInputHandler)
{
	Super::Initialize(InOwnerCharacter, InInputHandler);
}

void UUPPlayerLandState::EnterState()
{
	Super::EnterState();
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "Player Land Enter");

}

void UUPPlayerLandState::ExitState()
{
	Super::ExitState();
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Player Land Exit");

}

void UUPPlayerLandState::UpdateState()
{
	Super::UpdateState();
	if (InputHandler->IsMoving())
	{
		ChangeState(EPlayerStateType::Sprint);
	}
	else
	{
		ChangeState(EPlayerStateType::Idle);
	}
}
