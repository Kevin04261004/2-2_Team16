// Fill out your copyright notice in the Description page of Project Settings.


#include "State/UPPlayerBaseState.h"

UUPPlayerBaseState::UUPPlayerBaseState()
{
	
}

void UUPPlayerBaseState::Initialize(AUPPlayerCharacter* InOwnerCharacter)
{
	this->OwnerCharacter = InOwnerCharacter;
}

void UUPPlayerBaseState::EnterState()
{
}

void UUPPlayerBaseState::ExitState()
{
}

void UUPPlayerBaseState::UpdateState()
{
}
