// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/UPPettuCharacter.h"

AUPPettuCharacter::AUPPettuCharacter()
{
	MaxHp = 60000.0f;
	AttackDamage = 300.0f;
	AttackSpeed = 1.0f;
	MovementSpeed = 300.0f;
	MaxComboCount = 3.0f;
	BaseComboFrameRate = 60.0f;
	LastComboFrameRate = 120.0f;
	MaxPatternActivateTime = 5000.0f;
	MaxStunStack = 100.0f;
	StandardDistance = 15.0f;

	CurrentHp = MaxHp;
	CurrentPatternActivateTime = 0.0f;
	DistanceFromPlayer = 0.0f;
	CurrentStunStack = 0.0f;
	CurrentStunStack = 0.0f;
	DamageReceived = 1.0f;
	hasStatus = PettuStatus::Idle;
}
