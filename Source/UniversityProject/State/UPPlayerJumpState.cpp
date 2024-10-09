// Fill out your copyright notice in the Description page of Project Settings.


#include "State/UPPlayerJumpState.h"

#include "Skill/Player/UPSkillManagerComponent.h"

UUPPlayerJumpState::UUPPlayerJumpState()
{
	
}

void UUPPlayerJumpState::Initialize(AUPPlayerCharacter* InOwnerCharacter, class UUPInputHandlerComponent* InInputHandler)
{
	Super::Initialize(InOwnerCharacter, InInputHandler);

	InputHandler->OnBaseAttackInputed.AddUObject(this, &UUPPlayerJumpState::TryBaseAttack);
}

void UUPPlayerJumpState::EnterState()
{
	Super::EnterState();
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "Player Jump Enter");
	OwnerCharacter->Jump();
}

void UUPPlayerJumpState::ExitState()
{
	Super::ExitState();
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Player Jump Exit");

}

void UUPPlayerJumpState::UpdateState()
{
	Super::UpdateState();

	/* if Check */
	if (OwnerCharacter->CanJump())
	{
		if (InputHandler->IsMoving())
		{
			ChangeState(InputHandler->IsSprint() ? EPlayerStateType::Sprint : EPlayerStateType::Walk);
		}
		else
		{
			ChangeState(EPlayerStateType::Idle);
		}
	}
	
	/* Logic Update */
	OwnerCharacter->MovementComponent->Move(InputHandler->GetMovementVector());
}

void UUPPlayerJumpState::TryBaseAttack()
{
	if (OwnerCharacter->GetStateManager()->GetCurrentState() != EPlayerStateType::Jump || !OwnerCharacter->GetSkillManager()->CanUseSkill(EPlayerSkillType::TakeDown))
	{
		return;
	}
	ChangeState(EPlayerStateType::TakeDown);
}
