// Fill out your copyright notice in the Description page of Project Settings.


#include "State/UPPlayerTakeDamageState.h"

#include "Components/AutoTargetingComponent.h"
#include "Skill/Player/UPSkillManagerComponent.h"

UUPPlayerTakeDamageState::UUPPlayerTakeDamageState()
{
	
}

void UUPPlayerTakeDamageState::Initialize(AUPPlayerCharacter* InOwnerCharacter, class UUPInputHandlerComponent* InInputHandler)
{
	Super::Initialize(InOwnerCharacter, InInputHandler);
}

void UUPPlayerTakeDamageState::InitSkillData()
{
	Super::InitSkillData();

	ThisSkillType = EPlayerSkillType::Damaged;
}


void UUPPlayerTakeDamageState::EnterState()
{
	UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Stop(0.0f);

	UUPSkillData* skillData = OwnerCharacter->GetSkillManager()->GetSkill(ThisSkillType)->GetSkillData();
	AnimInstance->Montage_Play(skillData->GetSkillAnimation(), skillData->GetAnimationSpeed(OwnerCharacter->GetStat()->GetTotalStat().AttackSpeed));
	
	// Super::EnterState();

	// TODO: 타겟 오토타겟팅
	if (skillData->IsAutoTargetingSkill())
	{
		OwnerCharacter->GetSkillManager()->GetAutoTargetingComponent()->Activate();
	}
	SkillDuration = skillData->GetSkillDuration(OwnerCharacter->GetStat()->GetTotalStat().AttackSpeed);

	OwnerCharacter->GetWorld()->GetTimerManager().SetTimer(SkillEndTimerHandle, this, &UUPPlayerTakeDamageState::SkillFinished, SkillDuration, false);
	
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "Player Damaged Enter");

}

void UUPPlayerTakeDamageState::ExitState()
{
	Super::ExitState();
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Player Damaged Exit");
}

void UUPPlayerTakeDamageState::UpdateState()
{
	Super::UpdateState();

}

void UUPPlayerTakeDamageState::SkillFinished()
{
	Super::SkillFinished();
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
