// Fill out your copyright notice in the Description page of Project Settings.


#include "State/UPPlayerTakeDownState.h"

#include "Character/Weapon/UPPlayerCharacterWeapon.h"
#include "Character/Weapon/UPWeaponBase.h"
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
	bIsAlreadyStoped = false;
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
	if (CurrentMontage == nullptr)
	{
		return;
	}

	/* 공격 애니메이션 시작 */
	if (CurrentMontage == OwnerCharacter->GetSkillManager()->GetSkill(ThisSkillType)->GetSkillData()->GetSkillAnimation() && !bIsAlreadyStoped)
	{
		float CurrentPosition = AnimInstance->Montage_GetPosition(CurrentMontage);
		float MontageLength = OwnerCharacter->GetSkillManager()->GetSkill(ThisSkillType)->GetSkillData()->GetSkillAnimation()->GetPlayLength();

		if (CurrentPosition >= MontageLength - 0.01f)
		{
			AnimInstance->Montage_SetPlayRate(CurrentMontage, 0.0f);
			bIsAlreadyStoped = true;
		}
	}

	/* 공격 애니메이션이 중지됨, 콜리전 체크 및 물리 필요. */
	if (CurrentMontage == OwnerCharacter->GetSkillManager()->GetSkill(ThisSkillType)->GetSkillData()->GetSkillAnimation() && IsAnimationStoped())
	{
		OwnerCharacter->MovementComponent->Velocity = FVector(0, 0, -5000);
		FHitResult HitResult;
		AUPPlayerCharacterWeapon* playerWeapon = Cast<AUPPlayerCharacterWeapon>(OwnerCharacter->GetWeapon());

		playerWeapon->CheckAttackRange();
	}

	/* 바닥에 닿음. 이제, ToIdle 실행. */
	if (OwnerCharacter->CanJump() && IsAnimationStoped())
	{
		AnimInstance->Montage_Resume(CurrentMontage);
		AnimInstance->Montage_SetPlayRate(CurrentMontage, 1.0f);
		if (IsAnimationStoped())
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, "ERROR!!!!!!!!!!!!!");
		}
		PlayAttackToIdleMontage();
		SkillFinished();
	}
}
void UUPPlayerTakeDownState::SkillFinished()
{
	Super::SkillFinished();

	if (OwnerCharacter->MovementComponent->IsMovingOnGround())
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
	if (OwnerCharacter->GetStateManager()->TakeDownToIdleMontage != nullptr && AnimInstance != nullptr)
	{
		AnimInstance->Montage_Play(OwnerCharacter->GetStateManager()->TakeDownToIdleMontage);
	}
}

bool UUPPlayerTakeDownState::IsAnimationStoped() const
{
	UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
	UAnimMontage* CurrentMontage = AnimInstance->GetCurrentActiveMontage();
	return (AnimInstance->Montage_GetPlayRate(CurrentMontage) == 0.0f);
}

