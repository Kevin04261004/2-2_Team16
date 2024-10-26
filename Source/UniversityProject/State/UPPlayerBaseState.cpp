// Fill out your copyright notice in the Description page of Project Settings.


#include "State/UPPlayerBaseState.h"

UUPPlayerBaseState::UUPPlayerBaseState()
{
	
}

void UUPPlayerBaseState::Initialize(AUPPlayerCharacter* InOwnerCharacter, UUPInputHandlerComponent* InInputHandler)
{
	this->OwnerCharacter = InOwnerCharacter;
	this->InputHandler = InInputHandler;
}

void UUPPlayerBaseState::EnterState()
{
	
}

void UUPPlayerBaseState::ExitState()
{
	
}

void UUPPlayerBaseState::UpdateState()
{
	// TODO: 여기서 데미지로 State 변경
	// OwnerCharacter->OnTakeDamaged.AddLambda([]()
	// {
	// 	
	// });
}
