// Fill out your copyright notice in the Description page of Project Settings.


#include "State/UPPlayerSprintState.h"

#include "Character/UPPlayerCharacter.h"
#include "Components/UPInputHandlerComponent.h"
#include "Skill/Player/UPSkillManagerComponent.h"

UUPPlayerSprintState::UUPPlayerSprintState()
{
}

void UUPPlayerSprintState::Initialize(AUPPlayerCharacter* InOwnerCharacter, class UUPInputHandlerComponent* InInputHandler)
{
	Super::Initialize(InOwnerCharacter, InInputHandler);

	InputHandler->OnJumpInputed.AddUObject(this, &UUPPlayerSprintState::TryJump);
	InputHandler->OnDashInputed.AddUObject(this, &UUPPlayerSprintState::TryDash);
	InputHandler->OnBaseAttackInputed.AddUObject(this, &UUPPlayerSprintState::TryBaseAttack);
}

void UUPPlayerSprintState::EnterState()
{
	Super::EnterState();
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "Player Run Enter");

	OwnerCharacter->MovementComponent->SetIsSprinting(true);
}

void UUPPlayerSprintState::ExitState()
{
	Super::ExitState();
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Player Run Exit");

}

void UUPPlayerSprintState::UpdateState()
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

void UUPPlayerSprintState::TryJump()
{
	if (OwnerCharacter->GetStateManager()->GetCurrentState() != EPlayerStateType::Sprint || !OwnerCharacter->CanJump())
	{
		return;
	}
	ChangeState(EPlayerStateType::Jump);
}

void UUPPlayerSprintState::TryDash()
{
	if (OwnerCharacter->GetStateManager()->GetCurrentState() != EPlayerStateType::Sprint || !OwnerCharacter->GetSkillManager()->CanUseSkill(EPlayerSkillType::Dash))
	{
		return;
	}
	ChangeState(EPlayerStateType::Dash);
}

void UUPPlayerSprintState::TryBaseAttack()
{
	if (OwnerCharacter->GetStateManager()->GetCurrentState() != EPlayerStateType::Sprint || !OwnerCharacter->GetSkillManager()->CanUseSkill(EPlayerSkillType::UpperCut))
	{
		return;
	}
	ChangeState(EPlayerStateType::UpperCut);
}
