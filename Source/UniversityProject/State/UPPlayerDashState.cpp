// Fill out your copyright notice in the Description page of Project Settings.


#include "State/UPPlayerDashState.h"

#include "Skill/Player/UPDashSkill.h"
#include "Skill/Player/UPSkillManagerComponent.h"

UUPPlayerDashState::UUPPlayerDashState()
{
}

void UUPPlayerDashState::Initialize(AUPPlayerCharacter* InOwnerCharacter, class UUPInputHandlerComponent* InInputHandler)
{
	Super::Initialize(InOwnerCharacter, InInputHandler);
}

void UUPPlayerDashState::EnterState()
{
	Super::EnterState();
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "Player Dash Enter");
	OwnerCharacter->GetSkillManager()->UseSkill(EPlayerSkillType::Dash);

	UUPDashSkill* DashSkill = Cast<UUPDashSkill>(OwnerCharacter->GetSkillManager()->GetSkill(EPlayerSkillType::Dash));
	check(DashSkill != nullptr);

	if (!DashSkill->OnDashFinished.IsBoundToObject(this))
	{
		DashSkill->OnDashFinished.AddUObject(this, &UUPPlayerDashState::DashFinished);
	}
}

void UUPPlayerDashState::ExitState()
{
	Super::ExitState();
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Player Dash Exit");
}

void UUPPlayerDashState::UpdateState()
{
	Super::UpdateState();

	/* if Check */
	
	/* Logic Update */

}

void UUPPlayerDashState::DashFinished()
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
