// Fill out your copyright notice in the Description page of Project Settings.


#include "State/UPPlayerBaseState.h"

#include "Game/UPGameMode.h"

UUPPlayerBaseState::UUPPlayerBaseState()
{
	CheckConditionWhenStarted = EStageConditionType::None;
	CheckConditionWhenFinish = EStageConditionType::None;
}

void UUPPlayerBaseState::Initialize(AUPPlayerCharacter* InOwnerCharacter, UUPInputHandlerComponent* InInputHandler)
{
	this->OwnerCharacter = InOwnerCharacter;
	this->InputHandler = InInputHandler;
	
	OwnerCharacter->OnTakeDamaged.AddUObject(this, &UUPPlayerBaseState::TakeDamaged);
}

void UUPPlayerBaseState::EnterState()
{
	if (StageManager == nullptr)
	{
		AUPGameMode* gameMode = Cast<AUPGameMode>(OwnerCharacter->GetWorld()->GetAuthGameMode());
		if (gameMode == nullptr)
		{
			return;
		}
		StageManager = gameMode->StageManager;
		check (StageManager != nullptr);
	}
	StageManager->EvaluateCondition(CheckConditionWhenStarted);
}

void UUPPlayerBaseState::ExitState()
{
	StageManager->EvaluateCondition(CheckConditionWhenFinish);
}

void UUPPlayerBaseState::UpdateState()
{
	
}

void UUPPlayerBaseState::TakeDamaged(float amount)
{
	ChangeState(EPlayerStateType::Damaged);
}