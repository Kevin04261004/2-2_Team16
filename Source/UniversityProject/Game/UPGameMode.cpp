// Fill out your copyright notice in the Description page of Project Settings.


#include "UPGameMode.h"

#include "Audio/UPAudioManager.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Player/UPPlayerController.h"
#include "UI/UPHudWidget.h"
#include "UI/UPSkipTutorialWidget.h"

AUPGameMode::AUPGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Game/UniversityProject/BluePrints/Character/BP_PlayerCharacter.BP_PlayerCharacter_C"));
	check(DefaultPawnClassRef.Class != nullptr);
	DefaultPawnClass = DefaultPawnClassRef.Class;

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/UniversityProject.UPPlayerController"));
	check(PlayerControllerClassRef.Class != nullptr);
	PlayerControllerClass = PlayerControllerClassRef.Class;
	
	static ConstructorHelpers::FClassFinder<AUPStageManager> StageManagerClassRef(TEXT("/Game/UniversityProject/BluePrints/Stage/BP_UPStageManager.BP_UPStageManager_C"));
	if (StageManagerClassRef.Class != nullptr)
	{
		StageManagerClass = StageManagerClassRef.Class;
	}

	bIsCleared = false;
	bIsGameOver = false;
	bIsTutorialCleared = false;
}

void AUPGameMode::OnPlayerDead()
{
	AUPPlayerController* PlayerController = Cast<AUPPlayerController>(GetWorld()->GetFirstPlayerController());
	check(PlayerController != nullptr);
	PlayerController->GameOver();
	bIsGameOver = true;
}

void AUPGameMode::OnGameClear()
{
	AUPPlayerController* PlayerController = Cast<AUPPlayerController>(GetWorld()->GetFirstPlayerController());
	check(PlayerController != nullptr);
	PlayerController->GameClear();
	bIsCleared = true;
}

void AUPGameMode::OnTutorialClear()
{
	bIsTutorialCleared = true;
}

void AUPGameMode::RestartGame()
{
	StageManager->TutorialStartStage(StageManager->StageTutorialData->TutorialStages.Num() - 1);

	AUPPlayerController* PlayerController = Cast<AUPPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController == nullptr)
	{
		return;
	}	
	PlayerController->SkipTutorialWidget->SetVisibility(ESlateVisibility::Hidden);
	PlayerController->SetGameMode();
}

bool AUPGameMode::IsGameCleared()
{
	return bIsCleared;
}

void AUPGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (HudWidget != nullptr)
	{
		UUserWidget* Hud = CreateWidget<UUserWidget>(GetWorld(), HudWidget);
		HudWidgetObject = Cast<UUPHudWidget>(Hud);
		check(Hud != nullptr);
		Hud->AddToViewport();
		HudWidgetObject->SetVisibility(ESlateVisibility::Hidden);
		HudWidgetObject->SetPlayerHudVisible(false);
		HudWidgetObject->SetPettuHudVisible(false);
	}

	if (SettingWidget != nullptr)
	{
		UUPUserWidget* SettingWDG = CreateWidget<UUPUserWidget>(GetWorld(), SettingWidget);
		SettingWidgetObject = Cast<UUPSettingWidget>(SettingWDG);
		check(SettingWDG != nullptr);
		SettingWDG->AddToViewport();
		SettingWidgetObject->SetVisibility(ESlateVisibility::Hidden);
	}

	if (SkipTutorial != nullptr)
	{
		UUPUserWidget* SkipTutorialWDG = CreateWidget<UUPUserWidget>(GetWorld(), SkipTutorial);
		SkipTutorialObject = Cast<UUPSkipTutorialWidget>(SkipTutorialWDG);
		check(SkipTutorialWDG != nullptr);
		SkipTutorialWDG->AddToViewport();
		SkipTutorialObject->SetVisibility(ESlateVisibility::Hidden);
	}
	
	StageManager = GetWorld()->SpawnActor<AUPStageManager>(StageManagerClass, FVector::ZeroVector, FRotator::ZeroRotator);
	check(StageManager != nullptr);

	UUPAudioManager* AudioManager = GetGameInstance()->GetSubsystem<UUPAudioManager>();
	AudioManager->CollectAllSounds(GetWorld());
}


