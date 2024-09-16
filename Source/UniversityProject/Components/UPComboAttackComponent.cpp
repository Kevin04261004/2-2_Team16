// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/UPComboAttackComponent.h"

#include "UPCharacterStatComponent.h"
#include "Character/UPCharacterBase.h"
#include "GameData/UPComboActionData.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UUPComboAttackComponent::UUPComboAttackComponent()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboActionMontageRef(TEXT("/Game/UniversityProject/Animation/AM_ComboAttack.AM_ComboAttack"));
	check(ComboActionMontageRef.Object != nullptr);
	ComboActionMontage = ComboActionMontageRef.Object;
	
	static ConstructorHelpers::FObjectFinder<UUPComboActionData> ComboActionDataRef(TEXT("/Game/UniversityProject/CharacterAction/DA_ComboAttack.DA_ComboAttack"));
	check(ComboActionDataRef.Object != nullptr);
	ComboActionData = ComboActionDataRef.Object;

	OwningCharacter = Cast<AUPCharacterBase>(GetOwner());
}

void UUPComboAttackComponent::ProcessComboCommand()
{
	if (CurrentCombo == 0)
	{
		ComboActionBegin();
		return;
	}

	if (!ComboTimerHandle.IsValid())
	{
		HasNextComboCommand = false;
	}
	else
	{
		HasNextComboCommand = true;
	}
}

void UUPComboAttackComponent::ComboActionBegin()
{
	CurrentCombo = 1; 
	
	// Movement Setting
	UCharacterMovementComponent* movement = OwningCharacter->GetCharacterMovement();
	movement->SetMovementMode(EMovementMode::MOVE_None);
	
	// Animation Setting
	const float AttackSpeedRate = OwningCharacter->GetStat()->GetTotalStat().AttackSpeed;
	UAnimInstance* AnimInstance = OwningCharacter->GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(ComboActionMontage, AttackSpeedRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &UUPComboAttackComponent::ComboActionFinish);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboActionMontage);

	ComboTimerHandle.Invalidate();
	SetComboCheckTimer();
}

void UUPComboAttackComponent::ComboActionFinish(class UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	CurrentCombo = 0;
	UCharacterMovementComponent* movement = OwningCharacter->GetCharacterMovement();
	movement->SetMovementMode(EMovementMode::MOVE_Walking);

	OnComboAttackFinish.Broadcast();
}

void UUPComboAttackComponent::SetComboCheckTimer()
{
	int32 ComboIndex = CurrentCombo - 1;
	ensure(ComboActionData->EffectiveFrameCount.IsValidIndex(ComboIndex));

	const float AttackSpeedRate = OwningCharacter->GetStat()->GetTotalStat().AttackSpeed;
	float ComboEffectiveTime = (ComboActionData->EffectiveFrameCount[ComboIndex] / ComboActionData->FrameRate) / AttackSpeedRate;
	if (ComboEffectiveTime > 0.f)
	{
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &UUPComboAttackComponent::ComboCheck, ComboEffectiveTime, false);
	}
}

void UUPComboAttackComponent::ComboCheck()
{
	ComboTimerHandle.Invalidate();
	if (HasNextComboCommand)
	{
		UAnimInstance* AnimInstance = OwningCharacter->GetMesh()->GetAnimInstance();

		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboActionData->MaxComboCount);
		FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboActionData->MontageSectionNamePrefix, CurrentCombo);
		AnimInstance->Montage_JumpToSection(NextSection, ComboActionMontage);
		SetComboCheckTimer();
		HasNextComboCommand = false;
		
		OnComboStepEnd.Broadcast();
	}
}

