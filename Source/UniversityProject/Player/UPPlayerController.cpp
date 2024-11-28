// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/UPPlayerController.h"

#include "AI/NavigationSystemBase.h"
#include "Game/UPGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/UPTimeManager.h"
#include "UI/UPSkipTutorialWidget.h"

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
		HudWidgetObject = GameMode->HudWidgetObject;
		SkipTutorialWidget = GameMode->SkipTutorialObject;
		check(SettingWidgetObject != nullptr);
		check(HudWidgetObject != nullptr);
		check(SkipTutorialWidget != nullptr);

		GameMode->StageManager->OnBossStageStart.AddUObject(this, &AUPPlayerController::SetGameMode);
	}

	SetUIMode();
	SkipTutorialWidget->SetVisibility(ESlateVisibility::Visible);
}

void AUPPlayerController::SetUIMode() 
{
	// Set Input Mode
	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(SettingWidgetObject->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(InputModeData);

	// Ensure mouse cursor is shown
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	// Stop world time
	UUPTimeManager* TimeManager = GetGameInstance()->GetSubsystem<UUPTimeManager>();
	if (TimeManager)
	{
		TimeManager->WorldTimeStop();
	}
	
	// Update input mode state
	CurInputMode = EInputMode::UI;
}
void AUPPlayerController::SetGameMode()
{
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);

	UUPTimeManager* TimeManager = GetGameInstance()->GetSubsystem<UUPTimeManager>();
	TimeManager->WorldTimeReset();
	
	bShowMouseCursor = false;
	CurInputMode = EInputMode::Game;
	SettingWidgetObject->SetVisibility(ESlateVisibility::Hidden);
}