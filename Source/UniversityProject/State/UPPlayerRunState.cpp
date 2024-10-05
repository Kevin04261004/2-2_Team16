// Fill out your copyright notice in the Description page of Project Settings.


#include "State/UPPlayerRunState.h"

UUPPlayerRunState::UUPPlayerRunState()
{
}

void UUPPlayerRunState::Initialize(AUPPlayerCharacter* InOwnerCharacter)
{
	Super::Initialize(InOwnerCharacter);
}

void UUPPlayerRunState::EnterState()
{
	Super::EnterState();
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "Player Run Enter");
}

void UUPPlayerRunState::ExitState()
{
	Super::ExitState();
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Player Run Exit");

}

void UUPPlayerRunState::UpdateState()
{
	Super::UpdateState();
}
