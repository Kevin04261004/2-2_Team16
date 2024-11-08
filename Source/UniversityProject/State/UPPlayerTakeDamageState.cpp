// Fill out your copyright notice in the Description page of Project Settings.


#include "State/UPPlayerTakeDamageState.h"

UUPPlayerTakeDamageState::UUPPlayerTakeDamageState()
{
	
}

void UUPPlayerTakeDamageState::Initialize(AUPPlayerCharacter* InOwnerCharacter, class UUPInputHandlerComponent* InInputHandler)
{
	Super::Initialize(InOwnerCharacter, InInputHandler);
}

void UUPPlayerTakeDamageState::InitSkillData()
{
	Super::InitSkillData();

	ThisSkillType = EPlayerSkillType::Damaged;
}


void UUPPlayerTakeDamageState::EnterState()
{
	Super::EnterState();
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "Player Damaged Enter");

}

void UUPPlayerTakeDamageState::ExitState()
{
	Super::ExitState();
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Player Damaged Exit");

}

void UUPPlayerTakeDamageState::UpdateState()
{
	Super::UpdateState();

}

void UUPPlayerTakeDamageState::SkillFinished()
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
