// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/UPPlayerController.h"

#include "AI/NavigationSystemBase.h"
#include "Game/UPGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/UPTimeManager.h"

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

	// GameMode 가져오기
	AUPGameMode* GameMode = Cast<AUPGameMode>(UGameplayStatics::GetGameMode(this));
	if (GameMode)
	{
		SettingWidgetObject = GameMode->SettingWidgetObject;
		check(SettingWidgetObject != nullptr);
	}
}

void AUPPlayerController::SetUIMode()
{
	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(SettingWidgetObject->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(InputModeData);

	UUPTimeManager* TimeManager = GetGameInstance()->GetSubsystem<UUPTimeManager>();
	TimeManager->WorldTimeStop();
	
	SettingWidgetObject->SetVisibility(ESlateVisibility::Visible);
	bShowMouseCursor = true;
	CurInputMode = EInputMode::UI;
}

void AUPPlayerController::SetGameMode()
{
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);

	UUPTimeManager* TimeManager = GetGameInstance()->GetSubsystem<UUPTimeManager>();
	TimeManager->WorldTimeReset();
	
	SettingWidgetObject->SetVisibility(ESlateVisibility::Hidden);
	bShowMouseCursor = false;
	CurInputMode = EInputMode::Game;
}
