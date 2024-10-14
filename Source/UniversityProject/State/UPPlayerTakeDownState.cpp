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

void UUPPlayerTakeDownState::InitSkillData()
{
	Super::InitSkillData();

	ThisSkillType = EPlayerSkillType::TakeDown;
}

void UUPPlayerTakeDownState::EnterState()
{
	Super::EnterState();
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "Player TakeDown Enter");

	if (SkillEndTimerHandle.IsValid())
	{
		OwnerCharacter->GetWorld()->GetTimerManager().ClearTimer(SkillEndTimerHandle);
	}
}

void UUPPlayerTakeDownState::ExitState()
{
	Super::ExitState();
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Player TakeDown Exit");
}

void UUPPlayerTakeDownState::UpdateState()
{
	Super::UpdateState();

	UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
	UAnimMontage* CurrentMontage = AnimInstance->GetCurrentActiveMontage();
	if (CurrentMontage != nullptr)
	{
		if (CurrentMontage == OwnerCharacter->GetSkillManager()->GetSkill(ThisSkillType)->GetSkillData()->GetSkillAnimation() && !AnimInstance->Montage_GetIsStopped(CurrentMontage))
		{
			float CurrentPosition = AnimInstance->Montage_GetPosition(CurrentMontage);
			float MontageLength = OwnerCharacter->GetSkillManager()->GetSkill(ThisSkillType)->GetSkillData()->GetSkillAnimation()->GetPlayLength();

			if (CurrentPosition >= MontageLength - 0.01f)
			{
				AnimInstance->Montage_Pause(CurrentMontage);
			}
		}

		if (OwnerCharacter->CanJump())
		{
			PlayAttackToIdleMontage();
		}
	}
}
void UUPPlayerTakeDownState::SkillFinished()
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

void UUPPlayerTakeDownState::PlayAttackToIdleMontage()
{
	UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
	if (OwnerCharacter->TakeDownToIdleMontage != nullptr && AnimInstance != nullptr)
	{
		AnimInstance->Montage_Play(OwnerCharacter->TakeDownToIdleMontage);
		// float aniDuration = OwnerCharacter->TakeDownToIdleMontage->GetPlayLength() * OwnerCharacter->GetStat()->GetTotalStat().AttackSpeed;
		// FTimerHandle SkillEndTimerHandle2;
		// OwnerCharacter->GetWorld()->GetTimerManager().SetTimer(SkillEndTimerHandle2, this, &UUPPlayerTakeDownState::SkillFinished, aniDuration, false);
	}
}
