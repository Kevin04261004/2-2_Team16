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
#pragma optimize("", off)
void UUPPlayerComboAttackState::EnterState()
{
	Super::EnterState();
	float sec =  SkillDuration / ComboInputableData->DefaultFrameCount;
	OneFrameSec = sec;
	bIsAttackKeyDown = false;
	CurrentTime = 0.0f;
}
#pragma optimize("", on)
void UUPPlayerComboAttackState::ExitState()
{
	Super::ExitState();
}
#pragma optimize("", off)
void UUPPlayerComboAttackState::UpdateState()
{
	Super::UpdateState();
	
	CurrentTime += OwnerCharacter->GetWorld()->GetDeltaSeconds();

	float AnimationStartTime = OneFrameSec * ComboInputableData->AnimationChangeStartFrameCount;
	if (CurrentTime >= AnimationStartTime && bIsAttackKeyDown == true && NextAttackStateType != EPlayerStateType::None)
	{
		ChangeState(NextAttackStateType);
	}
}
#pragma optimize("", on)
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
