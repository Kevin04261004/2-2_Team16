// Fill out your copyright notice in the Description page of Project Settings.


#include "State/UPPlayerBaseAttack01State.h"

#include "Skill/Player/UPSkillManagerComponent.h"

UUPPlayerBaseAttack01State::UUPPlayerBaseAttack01State()
{
}

void UUPPlayerBaseAttack01State::Initialize(AUPPlayerCharacter* InOwnerCharacter, class UUPInputHandlerComponent* InInputHandler)
{
	Super::Initialize(InOwnerCharacter, InInputHandler);
	ComboInputableData = OwnerCharacter->ComboInputableData_Combo1;
}

void UUPPlayerBaseAttack01State::InitSkillData()
{
	Super::InitSkillData();

	ThisSkillType = EPlayerSkillType::BaseAttack01;
	NextAttackStateType = EPlayerStateType::BaseAttack02;
}

void UUPPlayerBaseAttack01State::EnterState()
{
	Super::EnterState();
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "Player BaseAttack01 Enter");
}

void UUPPlayerBaseAttack01State::ExitState()
{
	Super::ExitState();
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Player BaseAttack01 Exit");
}

void UUPPlayerBaseAttack01State::UpdateState()
{
	Super::UpdateState();
	
}

void UUPPlayerBaseAttack01State::SkillFinished()
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
