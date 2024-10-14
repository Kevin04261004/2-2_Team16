// Fill out your copyright notice in the Description page of Project Settings.


#include "State/UPPlayerUpperCutState.h"

#include "UPPlayerTakeDownState.h"
#include "Skill/Player/UPSkillManagerComponent.h"

class UUPSkillBase;

UUPPlayerUpperCutState::UUPPlayerUpperCutState()
{
}

void UUPPlayerUpperCutState::Initialize(AUPPlayerCharacter* InOwnerCharacter,
	class UUPInputHandlerComponent* InInputHandler)
{
	Super::Initialize(InOwnerCharacter, InInputHandler);
}

void UUPPlayerUpperCutState::InitSkillData()
{
	Super::InitSkillData();

	ThisSkillType = EPlayerSkillType::UpperCut;
}

void UUPPlayerUpperCutState::EnterState()
{
	Super::EnterState();

	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "Player Upper Cut Enter");
}

void UUPPlayerUpperCutState::ExitState()
{
	Super::ExitState();
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Player UpperCut Exit");
}

void UUPPlayerUpperCutState::UpdateState()
{
	Super::UpdateState();
	
}

void UUPPlayerUpperCutState::SkillFinished()
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
