// Fill out your copyright notice in the Description page of Project Settings.


#include "State/UPPlayerIdleState.h"

#include "Character/UPPlayerCharacter.h"
#include "Components/UPInputHandlerComponent.h"
#include "Skill/Player/UPSkillManagerComponent.h"

UUPPlayerIdleState::UUPPlayerIdleState()
{
}

void UUPPlayerIdleState::Initialize(AUPPlayerCharacter* InOwnerCharacter, class UUPInputHandlerComponent* InInputHandler)
{
	Super::Initialize(InOwnerCharacter, InInputHandler);

	InputHandler->OnJumpInputed.AddUObject(this, &UUPPlayerIdleState::TryJump);
	InputHandler->OnDashInputed.AddUObject(this, &UUPPlayerIdleState::TryDash);
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

void UUPPlayerIdleState::TryJump()
{
	if (OwnerCharacter->GetStateManager()->GetCurrentState() != EPlayerStateType::Idle || !OwnerCharacter->CanJump())
	{
		return;
	}
	ChangeState(EPlayerStateType::Jump);
}

void UUPPlayerIdleState::TryDash()
{
	if (OwnerCharacter->GetStateManager()->GetCurrentState() != EPlayerStateType::Idle || !OwnerCharacter->GetSkillManager()->CanUseSkill(EPlayerSkillType::Dash))
	{
		return;
	}
	ChangeState(EPlayerStateType::Dash);
}
