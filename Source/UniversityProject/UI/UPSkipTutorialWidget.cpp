// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UPSkipTutorialWidget.h"

#include "Components/Button.h"
#include "Game/UPGameMode.h"

class AUPPlayerController;

void UUPSkipTutorialWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (SkipTutorialButton != nullptr)
	{
		SkipTutorialButton->OnClicked.AddDynamic(this, &UUPSkipTutorialWidget::OnSkipTutorialButtonPressed);
	}
	if (NonSkipTutorialButton != nullptr)
	{
		NonSkipTutorialButton->OnClicked.AddDynamic(this, &UUPSkipTutorialWidget::OnNonSkipTutorialButtonPressed);
	}
}

void UUPSkipTutorialWidget::OnSkipTutorialButtonPressed()
{
	AUPGameMode* GameMode = Cast<AUPGameMode>(GetWorld()->GetAuthGameMode());
	GameMode->StageManager->TutorialStartStage(GameMode->StageManager->StageTutorialData->TutorialStages.Num() - 1);

	AUPPlayerController* PlayerController = Cast<AUPPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController == nullptr)
	{
		return;
	}	
	PlayerController->SkipTutorialWidget->SetVisibility(ESlateVisibility::Hidden);
	PlayerController->SetGameMode();
}

void UUPSkipTutorialWidget::OnNonSkipTutorialButtonPressed()
{
	AUPGameMode* GameMode = Cast<AUPGameMode>(GetWorld()->GetAuthGameMode());
	GameMode->StageManager->TutorialStartStage(0);
	AUPPlayerController* PlayerController = Cast<AUPPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController == nullptr)
	{
		return;
	}	
	PlayerController->SkipTutorialWidget->SetVisibility(ESlateVisibility::Hidden);
	PlayerController->SetGameMode();
}
