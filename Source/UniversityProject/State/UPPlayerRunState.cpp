// Fill out your copyright notice in the Description page of Project Settings.


#include "State/UPPlayerRunState.h"

#include "Character/UPPlayerCharacter.h"
#include "Components/UPInputHandlerComponent.h"

UUPPlayerRunState::UUPPlayerRunState()
{
}

void UUPPlayerRunState::Initialize(AUPPlayerCharacter* InOwnerCharacter, class UUPInputHandlerComponent* InInputHandler)
{
	Super::Initialize(InOwnerCharacter, InInputHandler);
}

void UUPPlayerRunState::EnterState()
{
	Super::EnterState();
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "Player Run Enter");

	OwnerCharacter->MovementComponent->SetIsSprinting(true);
}

void UUPPlayerRunState::ExitState()
{
	Super::ExitState();
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Player Run Exit");

}

void UUPPlayerRunState::UpdateState()
{
	Super::UpdateState();

	/* if Check */
	if (InputHandler->IsMoving())
	{
		if (!InputHandler->IsSprint())
		{
			ChangeState(EPlayerStateType::Walk);
		}
	}
	else
	{
		ChangeState(EPlayerStateType::Idle);
	}

	/* Logic Update */
	OwnerCharacter->MovementComponent->Move(InputHandler->GetMovementVector());
}
