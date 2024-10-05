// Fill out your copyright notice in the Description page of Project Settings.


#include "State/UPPlayerIdleState.h"

UUPPlayerIdleState::UUPPlayerIdleState()
{
}

void UUPPlayerIdleState::Initialize(AUPPlayerCharacter* InOwnerCharacter)
{
	Super::Initialize(InOwnerCharacter);
}

void UUPPlayerIdleState::EnterState()
{
	Super::EnterState();
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "Player Idle Enter");
}

void UUPPlayerIdleState::ExitState()
{
	Super::ExitState();
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Player Idle Exit");

}

void UUPPlayerIdleState::UpdateState()
{
	Super::UpdateState();
	
}
