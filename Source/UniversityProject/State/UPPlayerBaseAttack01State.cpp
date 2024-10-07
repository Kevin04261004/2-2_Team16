// Fill out your copyright notice in the Description page of Project Settings.


#include "State/UPPlayerBaseAttack01State.h"

#include "Skill/Player/UPSkillManagerComponent.h"

UUPPlayerBaseAttack01State::UUPPlayerBaseAttack01State()
{
}

void UUPPlayerBaseAttack01State::Initialize(AUPPlayerCharacter* InOwnerCharacter, class UUPInputHandlerComponent* InInputHandler)
{
	Super::Initialize(InOwnerCharacter, InInputHandler);
	OwnerCharacter->GetSkillManager()->UseSkill(EPlayerSkillType::BaseAttack01);
}

void UUPPlayerBaseAttack01State::EnterState()
{
	Super::EnterState();
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "Player BaseAttack01 Enter");

	UUPSkillBase* BaseAttack01 = OwnerCharacter->GetSkillManager()->GetSkill(EPlayerSkillType::BaseAttack01);
	check(BaseAttack01 != nullptr);

	OwnerCharacter->GetSkillManager()->UseSkill(EPlayerSkillType::BaseAttack01);
	
	float skillDuration = BaseAttack01->GetSkillData()->GetSkillDuration(OwnerCharacter->GetStat()->GetTotalStat().AttackSpeed);

	OwnerCharacter->GetWorld()->GetTimerManager().SetTimer(SkillEndTimerHandle, this, &UUPPlayerBaseAttack01State::Attack01End, skillDuration, false);
}

void UUPPlayerBaseAttack01State::ExitState()
{
	Super::ExitState();
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Player BaseAttack01 Exit");

	if (SkillEndTimerHandle.IsValid())
	{
		OwnerCharacter->GetWorld()->GetTimerManager().ClearTimer(SkillEndTimerHandle);
	}
}

void UUPPlayerBaseAttack01State::UpdateState()
{
	Super::UpdateState();
	
}

void UUPPlayerBaseAttack01State::TryDash()
{
	
}

void UUPPlayerBaseAttack01State::Attack01End()
{
	if (InputHandler->IsMoving())
	{
		ChangeState(InputHandler->IsSprint() ? EPlayerStateType::Sprint : EPlayerStateType::Walk);
	}
	else
	{
		ChangeState(EPlayerStateType::Idle);
	}
	//
	// if (OwnerCharacter->CanJump())
	// {
	//
	// }
	// else
	// {
	// 	// TODO: InAir State
	// }
}
