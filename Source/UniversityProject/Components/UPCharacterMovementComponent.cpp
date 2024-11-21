// Fill out your copyright notice in the Description page of Project Settings.


#include "UPCharacterMovementComponent.h"

#include "Character/UPCharacterBase.h"
#include "Character/UPPlayerCharacter.h"

UUPCharacterMovementComponent::UUPCharacterMovementComponent()
{
	bIsSprinting = false;
}

void UUPCharacterMovementComponent::Initialize()
{
	OwningCharacter = Cast<AUPCharacterBase>(GetOwner());

	
	AUPPlayerCharacter* PlayerCharacter = Cast<AUPPlayerCharacter>(OwningCharacter);
	if (PlayerCharacter != nullptr)
	{
		PlayerCharacter->GetInputHandler()->OnMoveInputed.AddUObject(this, &UUPCharacterMovementComponent::SetLastInput);
		return;
	}
}


void UUPCharacterMovementComponent::SetIsSprinting(bool isSprinting)
{
	if (StatComponent == nullptr)
	{
		return;
	}
	bIsSprinting = isSprinting;
	FUPCharacterStat Stat = StatComponent->GetTotalStat();
	DesiredSpeed = bIsSprinting ? Stat.SprintSpeed : Stat.WalkSpeed;

	if (!GetWorld()->GetTimerManager().IsTimerActive(SpeedChangeTimerHandle))
	{
		GetWorld()->GetTimerManager().SetTimer(SpeedChangeTimerHandle, this, &UUPCharacterMovementComponent::UpdateSpeed, GetWorld()->GetDeltaSeconds(), true);
	}
}

void UUPCharacterMovementComponent::SetCharacterCanMove(bool bCanMove)
{
	MaxWalkSpeed = bCanMove ? MaxWalkSpeed : 0;
}

void UUPCharacterMovementComponent::SetCharacterStat(UUPCharacterStatComponent* InStatComponent)
{
	this->StatComponent = InStatComponent;
	check(StatComponent != nullptr);
}

void UUPCharacterMovementComponent::Move(FVector2D MovementVector)
{
	if (MovementVector == FVector2D::ZeroVector)
	{
		return;
	}
	const FRotator Rotation = GetController()->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	OwningCharacter->AddMovementInput(ForwardDirection, MovementVector.X);
	OwningCharacter->AddMovementInput(RightDirection, MovementVector.Y);
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

void UUPCharacterMovementComponent::SetLastInput(FVector2D MovementVector)
{
	const FRotator Rotation = GetController()->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	if (!MovementVector.IsNearlyZero())
	{
		LastInputVector = (ForwardDirection * MovementVector.X) + (RightDirection * MovementVector.Y);
	}
}
