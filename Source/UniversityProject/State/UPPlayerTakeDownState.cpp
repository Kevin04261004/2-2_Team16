// Fill out your copyright notice in the Description page of Project Settings.


#include "State/UPPlayerTakeDownState.h"

#include "Skill/Player/UPSkillManagerComponent.h"

class UUPSkillBase;

UUPPlayerTakeDownState::UUPPlayerTakeDownState()
{
}

void UUPPlayerTakeDownState::Initialize(AUPPlayerCharacter* InOwnerCharacter,
	class UUPInputHandlerComponent* InInputHandler)
{
	Super::Initialize(InOwnerCharacter, InInputHandler);
}

void UUPPlayerTakeDownState::InitSkillData()
{
	Super::InitSkillData();

	ThisSkillType = EPlayerSkillType::TakeDown;
}

void UUPPlayerTakeDownState::EnterState()
{
	Super::EnterState();
	
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "Player TakeDown Enter");
}

void UUPPlayerTakeDownState::ExitState()
{
	Super::ExitState();
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Player TakeDown Exit");
}

void UUPPlayerTakeDownState::UpdateState()
{
	Super::UpdateState();
}

void UUPPlayerTakeDownState::TryDash()
{
	
}

void UUPPlayerTakeDownState::SkillFinished()
{
	Super::SkillFinished();

	if (InputHandler->IsMoving())
	{
		ChangeState(InputHandler->IsSprint() ? EPlayerStateType::Sprint : EPlayerStateType::Walk);
	}
	else
	{
		ChangeState(EPlayerStateType::Idle);
	}
}
