// Fill out your copyright notice in the Description page of Project Settings.


#include "UPGameMode.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Player/UPPlayerController.h"
#include "UI/UPHudWidget.h"

AUPGameMode::AUPGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Game/UniversityProject/BluePrints/Character/BP_PlayerCharacter.BP_PlayerCharacter_C"));
	check(DefaultPawnClassRef.Class != nullptr);
	DefaultPawnClass = DefaultPawnClassRef.Class;

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/UniversityProject.UPPlayerController"));
	check(PlayerControllerClassRef.Class != nullptr);
	PlayerControllerClass = PlayerControllerClassRef.Class;

	bIsCleared = false;
}

void AUPGameMode::OnPlayerDead()
{
	AUPPlayerController* PlayerController = Cast<AUPPlayerController>(GetWorld()->GetFirstPlayerController());
	check(PlayerController != nullptr);
	PlayerController->GameOver();
}

bool AUPGameMode::IsGameCleared()
{
	return bIsCleared;
}

void AUPGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (HudWidget)
	{
		UUserWidget* Hud = CreateWidget<UUserWidget>(GetWorld(), HudWidget);
		HudWidgetObject = Cast<UUPHudWidget>(Hud);
		check(Hud != nullptr);
		Hud->AddToViewport();
	}
}
