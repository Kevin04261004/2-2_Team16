// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UPGameInstance.h"

#include "Blueprint/UserWidget.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Kismet/GameplayStatics.h"
#include "Player/UPPlayerController.h"

void UUPGameInstance::StartGame()
{
	if (LoadingWidgetClass)
	{
		LoadingWidgetInstance = CreateWidget<UUserWidget>(this, LoadingWidgetClass);
		if (LoadingWidgetInstance)
		{
			LoadingWidgetInstance->AddToViewport();
		}
	}

	FString MapPath = TEXT("/Game/UniversityProject/Level/BossLevel");
	LoadPackageAsync(MapPath, FLoadPackageAsyncDelegate::CreateUObject(this, &UUPGameInstance::OnMapLoaded));
}

void UUPGameInstance::OnMapLoaded(const FName& PackageName, UPackage* LoadedPackage, EAsyncLoadingResult::Type Result)
{
	if (Result == EAsyncLoadingResult::Succeeded)
	{
		if (LoadingWidgetInstance)
		{
			LoadingWidgetInstance->RemoveFromParent();
		}
	}

	UGameplayStatics::OpenLevel(GetWorld(), FName(*PackageName.ToString()));
	AUPPlayerController* PlayerController = Cast<AUPPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PlayerController)
	{
		PlayerController->ClientSetCameraFade(true, FColor::Black, FVector2D(1.0f, 0.0f), 1.0f, false, true);
	}

	else
	{
		// Handle loading failure if needed
		UE_LOG(LogTemp, Error, TEXT("Failed to load level: %s"), *PackageName.ToString());
	}
}
