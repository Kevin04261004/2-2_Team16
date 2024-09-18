// Fill out your copyright notice in the Description page of Project Settings.


#include "UPCharacterMovementComponent.h"

UUPCharacterMovementComponent::UUPCharacterMovementComponent()
{
	bIsSprinting = false;
}

void UUPCharacterMovementComponent::SetIsSprinting(bool isSprinting)
{
	check(StatComponent != nullptr);
	bIsSprinting = isSprinting;

	FUPCharacterStat Stat = StatComponent->GetTotalStat();
	DesiredSpeed = bIsSprinting ? Stat.SprintSpeed : Stat.WalkSpeed;

	if (!GetWorld()->GetTimerManager().IsTimerActive(SpeedChangeTimerHandle))
	{
		GetWorld()->GetTimerManager().SetTimer(SpeedChangeTimerHandle, this, &UUPCharacterMovementComponent::UpdateSpeed, GetWorld()->GetDeltaSeconds(), true);
	}
}

void UUPCharacterMovementComponent::SetCharacterStat(UUPCharacterStatComponent* InStatComponent)
{
	this->StatComponent = InStatComponent;
	check(StatComponent != nullptr);
}

void UUPCharacterMovementComponent::UpdateSpeed()
{
	float CurrentSpeed = MaxWalkSpeed;
	float NewSpeed = FMath::FInterpTo(CurrentSpeed, DesiredSpeed, GetWorld()->GetDeltaSeconds(), SpeedChangeRate);
	MaxWalkSpeed = NewSpeed;

	if (FMath::IsNearlyEqual(NewSpeed, DesiredSpeed, 1.0f))
	{
		GetWorld()->GetTimerManager().ClearTimer(SpeedChangeTimerHandle);
	}
}
