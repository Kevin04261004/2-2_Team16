// Fill out your copyright notice in the Description page of Project Settings.


#include "State/UPPlayerBaseSkillState.h"

#include "Skill/Player/UPSkillManagerComponent.h"

UUPPlayerBaseSkillState::UUPPlayerBaseSkillState()
{
}

void UUPPlayerBaseSkillState::Initialize(AUPPlayerCharacter* InOwnerCharacter,
	class UUPInputHandlerComponent* InInputHandler)
{
	Super::Initialize(InOwnerCharacter, InInputHandler);
	InitSkillData();
}

void UUPPlayerBaseSkillState::InitSkillData()
{
	// TODO: Set ThisSkillType here!!!
}

void UUPPlayerBaseSkillState::EnterState()
{
	Super::EnterState();
	UUPSkillBase* ThisSkill = OwnerCharacter->GetSkillManager()->GetSkill(ThisSkillType);
	check(ThisSkill != nullptr);

	OwnerCharacter->GetSkillManager()->UseSkill(ThisSkillType);
	
	SkillDuration = ThisSkill->GetSkillData()->GetSkillDuration(OwnerCharacter->GetStat()->GetTotalStat().AttackSpeed);

	OwnerCharacter->GetWorld()->GetTimerManager().SetTimer(SkillEndTimerHandle, this, &UUPPlayerBaseSkillState::SkillFinished, SkillDuration, false);
}

void UUPPlayerBaseSkillState::ExitState()
{
	Super::ExitState();
	if (SkillEndTimerHandle.IsValid())
	{
		OwnerCharacter->GetWorld()->GetTimerManager().ClearTimer(SkillEndTimerHandle);
	}
}

void UUPPlayerBaseSkillState::UpdateState()
{
	Super::UpdateState();
}

void UUPPlayerBaseSkillState::SkillFinished()
{
	// TODO: Set Skill Finish GOTO here!!!
}
