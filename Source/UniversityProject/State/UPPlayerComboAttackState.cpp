// Fill out your copyright notice in the Description page of Project Settings.


#include "State/UPPlayerComboAttackState.h"

UUPPlayerComboAttackState::UUPPlayerComboAttackState()
{
	OneFrameSec = -1.0f;
}

void UUPPlayerComboAttackState::Initialize(AUPPlayerCharacter* InOwnerCharacter, class UUPInputHandlerComponent* InInputHandler)
{
	Super::Initialize(InOwnerCharacter, InInputHandler);
	InputHandler->OnBaseAttackInputed.AddUObject(this, &UUPPlayerComboAttackState::TryBaseAttack);
}

void UUPPlayerComboAttackState::InitSkillData()
{
	Super::InitSkillData();

	// TODO: Set Next State Here
	NextAttackStateType = EPlayerStateType::None;
}

void UUPPlayerComboAttackState::EnterState()
{
	Super::EnterState();

}

void UUPPlayerComboAttackState::ExitState()
{
	Super::ExitState();
}

void UUPPlayerComboAttackState::UpdateState()
{
	Super::UpdateState();
	
	if (CurrentTime >= ComboAnimationStartTime && bIsAttackKeyDown == true && NextAttackStateType != EPlayerStateType::None)
	{
		ChangeState(NextAttackStateType);
	}
}

void UUPPlayerComboAttackState::SkillFinished()
{
	Super::SkillFinished();
}

void UUPPlayerComboAttackState::TryBaseAttack()
{
	if (CurrentTime <= OneFrameSec * ComboInputableData->InputAllowFrameCount)
	{
		bIsAttackKeyDown = true;
	}
}
