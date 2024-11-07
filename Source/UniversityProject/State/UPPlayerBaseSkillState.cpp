// Fill out your copyright notice in the Description page of Project Settings.


#include "State/UPPlayerBaseSkillState.h"

#include "GameData/UPComboInputableData.h"
#include "Skill/Player/UPSkillManagerComponent.h"

UUPPlayerBaseSkillState::UUPPlayerBaseSkillState()
{
}

void UUPPlayerBaseSkillState::Initialize(AUPPlayerCharacter* InOwnerCharacter,
	class UUPInputHandlerComponent* InInputHandler)
{
	Super::Initialize(InOwnerCharacter, InInputHandler);
	InputHandler->OnDashInputed.AddUObject(this, &UUPPlayerBaseSkillState::TryDash);
	InputHandler->OnJumpInputed.AddUObject(this, &UUPPlayerBaseSkillState::TryJump);
	InitSkillData();
}

void UUPPlayerBaseSkillState::InitSkillData()
{
	// TODO: Set ThisSkillType here!!!
}

// #pragma optimize("", off)
void UUPPlayerBaseSkillState::EnterState()
{
	Super::EnterState();
	UUPSkillBase* ThisSkill = OwnerCharacter->GetSkillManager()->GetSkill(ThisSkillType);
	check(ThisSkill != nullptr);

	OwnerCharacter->GetSkillManager()->UseSkill(ThisSkillType);
	
	SkillDuration = ThisSkill->GetSkillData()->GetSkillDuration(OwnerCharacter->GetStat()->GetTotalStat().AttackSpeed);

	OwnerCharacter->GetWorld()->GetTimerManager().SetTimer(SkillEndTimerHandle, this, &UUPPlayerBaseSkillState::SkillFinished, SkillDuration, false);

	/* Frame */
	if (ComboInputableData != nullptr)
	{
		float sec =  SkillDuration / ComboInputableData->DefaultFrameCount;
		OneFrameSec = sec;
		bIsAttackKeyDown = false;
		bIsDashKeyDown = false;
		bIsJumpKeyDown = false;
		CurrentTime = 0.0f;
		ComboAnimationStartTime = OneFrameSec * ComboInputableData->AnimationChangeStartFrameCount;	
	}
}
// #pragma optimize("", on)


void UUPPlayerBaseSkillState::ExitState()
{
	Super::ExitState();
	if (SkillEndTimerHandle.IsValid())
	{
		OwnerCharacter->GetWorld()->GetTimerManager().ClearTimer(SkillEndTimerHandle);
	}
	bIsDashKeyDown = false;
	bIsAttackKeyDown = false;
	bIsJumpKeyDown = false;
}

#pragma optimize("", off)
// PRAGMA_DISABLE_OPTIMIZATION
void UUPPlayerBaseSkillState::UpdateState()
{
	Super::UpdateState();
	CurrentTime += OwnerCharacter->GetWorld()->GetDeltaSeconds();
	
	if (ComboInputableData != nullptr && bIsDashKeyDown == true && ComboInputableData->bCanDash && (!ComboInputableData->bDashAfterAnimationChangeStartFrame || CurrentTime >= ComboAnimationStartTime))
	{
		ChangeState(EPlayerStateType::Dash);
	}
	
	if (ComboInputableData != nullptr && bIsJumpKeyDown == true && ComboInputableData->bCanJump && (!ComboInputableData->bJumpAfterAnimationChangeStartFrame || CurrentTime >= ComboAnimationStartTime))
	{
		ChangeState(EPlayerStateType::Jump);
	}
}
#pragma optimize("", on)

void UUPPlayerBaseSkillState::SkillFinished()
{
	// TODO: Set Skill Finish GOTO here!!!
}

void UUPPlayerBaseSkillState::TryDash()
{
	bIsDashKeyDown = true;
}

void UUPPlayerBaseSkillState::TryJump()
{
	bIsJumpKeyDown = true;
}
