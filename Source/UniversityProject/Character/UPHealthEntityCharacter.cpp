// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/UPHealthEntityCharacter.h"

#include "Components/UPCharacterMovementComponent.h"

AUPHealthEntityCharacter::AUPHealthEntityCharacter(const FObjectInitializer& ObjectInitializer) : CurAttackDamage(0), bIsDead(false)
{
	
}

float AUPHealthEntityCharacter::UPTakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	StatComponent->ApplyDamage(DamageAmount);
	OnTakeDamaged.Broadcast(DamageAmount);
	return DamageAmount;
}

void AUPHealthEntityCharacter::SetDead()
{
	GetCharacterMovement()->SetMovementMode(MOVE_None);
	PlayDeadAnimation();
	SetActorEnableCollision(false);
	bIsDead = true;
}

void AUPHealthEntityCharacter::PlayDeadAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance == nullptr)
	{
		return;
	}
	AnimInstance->StopAllMontages(0.0f);
	if (DeadMontage == nullptr)
	{
		Destroy();
		return;
	}
	AnimInstance->Montage_Play(DeadMontage, 1.0f);
	AnimInstance->OnMontageEnded.AddDynamic(this, &AUPHealthEntityCharacter::DeadAnimEnd);
}

void AUPHealthEntityCharacter::DeadAnimEnd(UAnimMontage* Montage, bool bInterrupted)
{
	GetMesh()->GetAnimInstance()->OnMontageEnded.RemoveDynamic(this, &AUPHealthEntityCharacter::DeadAnimEnd);
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	bIsDead = false;
}