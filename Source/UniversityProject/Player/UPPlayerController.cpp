// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/UPPlayerController.h"

AUPPlayerController::AUPPlayerController()
{
}

void AUPPlayerController::GameOver()
{
	K2_OnGameOver();
}

void AUPPlayerController::GameClear()
{
	K2_OnGameClear();
}

void AUPPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
}
