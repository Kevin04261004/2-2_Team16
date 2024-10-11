// Fill out your copyright notice in the Description page of Project Settings.


#include "State/UPPlayerInAirState.h"

#include "Skill/Player/UPSkillManagerComponent.h"

UUPPlayerInAirState::UUPPlayerInAirState()
{
}

void UUPPlayerInAirState::Initialize(AUPPlayerCharacter* InOwnerCharacter,
	class UUPInputHandlerComponent* InInputHandler)
{
	Super::Initialize(InOwnerCharacter, InInputHandler);

	InputHandler->OnBaseAttackInputed.AddUObject(this, &UUPPlayerInAirState::TryBaseAttack);
}

void UUPPlayerInAirState::EnterState()
{
	Super::EnterState();
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "Player InAir Enter");
}

void UUPPlayerInAirState::ExitState()
{
	Super::ExitState();
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Player InAir Exit");
}

void UUPPlayerInAirState::UpdateState()
{
	Super::UpdateState();
	
	/* if Check */
	if (OwnerCharacter->CanJump())
	{
		ChangeState(EPlayerStateType::Land);
	}
	
	/* Logic Update */
	OwnerCharacter->MovementComponent->Move(InputHandler->GetMovementVector());
}

void UUPPlayerInAirState::TryBaseAttack()
{
	if (OwnerCharacter->GetStateManager()->GetCurrentState() != EPlayerStateType::InAir
	|| !OwnerCharacter->GetSkillManager()->CanUseSkill(EPlayerSkillType::TakeDown)
	|| OwnerCharacter->MovementComponent->Velocity.Z > 0)
	{
		return;
	}
	ChangeState(EPlayerStateType::TakeDown);
}
