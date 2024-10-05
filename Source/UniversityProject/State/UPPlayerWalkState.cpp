// Fill out your copyright notice in the Description page of Project Settings.


#include "State/UPPlayerWalkState.h"

UUPPlayerWalkState::UUPPlayerWalkState()
{
}

void UUPPlayerWalkState::Initialize(AUPPlayerCharacter* InOwnerCharacter)
{
	Super::Initialize(InOwnerCharacter);
}

void UUPPlayerWalkState::EnterState()
{
	Super::EnterState();
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "Player Walk Enter");
}

void UUPPlayerWalkState::ExitState()
{
	Super::ExitState();
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Player Walk Exit");

}

void UUPPlayerWalkState::UpdateState()
{
	Super::UpdateState();
}
