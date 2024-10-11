// Fill out your copyright notice in the Description page of Project Settings.


#include "State/UPPlayerBaseAttack02State.h"


UUPPlayerBaseAttack02State::UUPPlayerBaseAttack02State()
{
	
}

void UUPPlayerBaseAttack02State::Initialize(AUPPlayerCharacter* InOwnerCharacter, class UUPInputHandlerComponent* InInputHandler)
{
	Super::Initialize(InOwnerCharacter, InInputHandler);
}

void UUPPlayerBaseAttack02State::InitSkillData()
{
	Super::InitSkillData();

	ThisSkillType = EPlayerSkillType::BaseAttack02;
	NextAttackStateType = EPlayerStateType::BaseAttack03;
}

void UUPPlayerBaseAttack02State::EnterState()
{
	Super::EnterState();
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "Player BaseAttack02 Enter");
}

void UUPPlayerBaseAttack02State::ExitState()
{
	Super::ExitState();
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Player BaseAttack02 Exit");
}

void UUPPlayerBaseAttack02State::UpdateState()
{
	Super::UpdateState();
	
}

void UUPPlayerBaseAttack02State::SkillFinished()
{
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