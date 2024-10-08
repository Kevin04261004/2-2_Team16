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

void UUPPlayerUpperCutState::EnterState()
{
	Super::EnterState();

	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "Player Upper Cut Enter");

	UUPSkillBase* UpperCutAttack = OwnerCharacter->GetSkillManager()->GetSkill(EPlayerSkillType::UpperCut);
	check(UpperCutAttack != nullptr);

	OwnerCharacter->GetSkillManager()->UseSkill(EPlayerSkillType::UpperCut);
	
	float skillDuration = UpperCutAttack->GetSkillData()->GetSkillDuration(OwnerCharacter->GetStat()->GetTotalStat().AttackSpeed);

	OwnerCharacter->GetWorld()->GetTimerManager().SetTimer(SkillEndTimerHandle, this, &UUPPlayerUpperCutState::UpperAttackEnd, skillDuration, false);
}

void UUPPlayerUpperCutState::ExitState()
{
	Super::ExitState();
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Player UpperCut Exit");

	if (SkillEndTimerHandle.IsValid())
	{
		OwnerCharacter->GetWorld()->GetTimerManager().ClearTimer(SkillEndTimerHandle);
	}
}

void UUPPlayerUpperCutState::UpdateState()
{
	Super::UpdateState();
}

void UUPPlayerUpperCutState::UpperAttackEnd()
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
