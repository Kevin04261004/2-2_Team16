// Fill out your copyright notice in the Description page of Project Settings.


#include "State/UPPlayerWalkState.h"

#include "Skill/Player/UPSkillManagerComponent.h"

UUPPlayerWalkState::UUPPlayerWalkState()
{
}

void UUPPlayerWalkState::Initialize(AUPPlayerCharacter* InOwnerCharacter, class UUPInputHandlerComponent* InInputHandler)
{
	Super::Initialize(InOwnerCharacter, InInputHandler);

	InInputHandler->OnJumpInputed.AddUObject(this, &UUPPlayerWalkState::TryJump);
	InputHandler->OnDashInputed.AddUObject(this, &UUPPlayerWalkState::TryDash);
}

void UUPPlayerWalkState::EnterState()
{
	Super::EnterState();
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "Player Walk Enter");

	OwnerCharacter->MovementComponent->SetIsSprinting(false);
}

void UUPPlayerWalkState::ExitState()
{
	Super::ExitState();
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Player Walk Exit");

}

void UUPPlayerWalkState::UpdateState()
{
	Super::UpdateState();

	/* if Check */
	if (InputHandler->IsMoving())
	{
		if (InputHandler->IsSprint())
		{
			ChangeState(EPlayerStateType::Sprint);
		}
	}
	else
	{
		ChangeState(EPlayerStateType::Idle);
	}

	/* Logic Update */
	OwnerCharacter->MovementComponent->Move(InputHandler->GetMovementVector());
}

void UUPPlayerWalkState::TryJump()
{
	if (OwnerCharacter->GetStateManager()->GetCurrentState() != EPlayerStateType::Walk || !OwnerCharacter->CanJump())
	{
		return;
	}
	ChangeState(EPlayerStateType::Jump);
}

void UUPPlayerWalkState::TryDash()
{
	if (OwnerCharacter->GetStateManager()->GetCurrentState() != EPlayerStateType::Walk || !OwnerCharacter->GetSkillManager()->CanUseSkill(EPlayerSkillType::Dash))
	{
		return;
	}
	ChangeState(EPlayerStateType::Dash);
}
