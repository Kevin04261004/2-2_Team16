// Fill out your copyright notice in the Description page of Project Settings.


#include "State/UPPlayerBaseAttack03State.h"


UUPPlayerBaseAttack03State::UUPPlayerBaseAttack03State()
{
	CheckConditionWhenStarted = EStageConditionType::UseBaseAttack03;
	CheckConditionWhenFinish = EStageConditionType::FinishUseBaseAttack03;
}

void UUPPlayerBaseAttack03State::Initialize(AUPPlayerCharacter* InOwnerCharacter, class UUPInputHandlerComponent* InInputHandler)
{
	Super::Initialize(InOwnerCharacter, InInputHandler);
	ComboInputableData = OwnerCharacter->GetStateManager()->ComboInputableData_Combo3;
}

void UUPPlayerBaseAttack03State::InitSkillData()
{
	Super::InitSkillData();

	ThisSkillType = EPlayerSkillType::BaseAttack03;
	NextAttackStateType = EPlayerStateType::BaseAttack01;
}

void UUPPlayerBaseAttack03State::EnterState()
{
	Super::EnterState();
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "Player BaseAttack03 Enter");
}

void UUPPlayerBaseAttack03State::ExitState()
{
	Super::ExitState();
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Player BaseAttack03 Exit");
}

void UUPPlayerBaseAttack03State::UpdateState()
{
	Super::UpdateState();
	
}

void UUPPlayerBaseAttack03State::SkillFinished()
{
	if (OwnerCharacter->MovementComponent->IsMovingOnGround())
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