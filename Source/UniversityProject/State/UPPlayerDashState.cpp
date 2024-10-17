// Fill out your copyright notice in the Description page of Project Settings.


#include "State/UPPlayerDashState.h"

#include "Components/PhysicsControlComponent.h"
#include "Components/UPCameraComponent.h"
#include "Skill/Player/UPSkillManagerComponent.h"

UUPPlayerDashState::UUPPlayerDashState()
{
}

void UUPPlayerDashState::Initialize(AUPPlayerCharacter* InOwnerCharacter, class UUPInputHandlerComponent* InInputHandler)
{
	Super::Initialize(InOwnerCharacter, InInputHandler);
}

void UUPPlayerDashState::InitSkillData()
{
	Super::InitSkillData();

	ThisSkillType = EPlayerSkillType::Dash;
}

void UUPPlayerDashState::EnterState()
{
	Super::EnterState();
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "Player Dash Enter");
	OwnerCharacter->GetCameraComponent()->DashShakeCamera();
	OwnerCharacter->PhysicsControlComponent->CollisionOff();
}

void UUPPlayerDashState::ExitState()
{
	Super::ExitState();
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Player Dash Exit");
	OwnerCharacter->PhysicsControlComponent->CollisionOn();
}

void UUPPlayerDashState::UpdateState()
{
	Super::UpdateState();

	/* if Check */
	
	/* Logic Update */

}

void UUPPlayerDashState::SkillFinished()
{
	Super::SkillFinished();
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
	else
	{
		ChangeState(EPlayerStateType::InAir);
	}
}
