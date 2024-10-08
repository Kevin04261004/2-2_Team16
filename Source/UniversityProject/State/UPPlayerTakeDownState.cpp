// Fill out your copyright notice in the Description page of Project Settings.


#include "State/UPPlayerTakeDownState.h"

#include "Skill/Player/UPSkillManagerComponent.h"

class UUPSkillBase;

UUPPlayerTakeDownState::UUPPlayerTakeDownState()
{
}

void UUPPlayerTakeDownState::Initialize(AUPPlayerCharacter* InOwnerCharacter,
	class UUPInputHandlerComponent* InInputHandler)
{
	Super::Initialize(InOwnerCharacter, InInputHandler);
}

void UUPPlayerTakeDownState::EnterState()
{
	Super::EnterState();
	
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "Player TakeDown Enter");

	UUPSkillBase* TakeDownAttack = OwnerCharacter->GetSkillManager()->GetSkill(EPlayerSkillType::TakeDown);
	check(TakeDownAttack != nullptr);

	OwnerCharacter->GetSkillManager()->UseSkill(EPlayerSkillType::TakeDown);
	
	float skillDuration = TakeDownAttack->GetSkillData()->GetSkillDuration(OwnerCharacter->GetStat()->GetTotalStat().AttackSpeed);

	OwnerCharacter->GetWorld()->GetTimerManager().SetTimer(SkillEndTimerHandle, this, &UUPPlayerTakeDownState::TakeDownEnd, skillDuration, false);
}

void UUPPlayerTakeDownState::ExitState()
{
	Super::ExitState();
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Player TakeDown Exit");

	if (SkillEndTimerHandle.IsValid())
	{
		OwnerCharacter->GetWorld()->GetTimerManager().ClearTimer(SkillEndTimerHandle);
	}
}

void UUPPlayerTakeDownState::UpdateState()
{
	Super::UpdateState();
}

void UUPPlayerTakeDownState::TryDash()
{
	
}

void UUPPlayerTakeDownState::TakeDownEnd()
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
