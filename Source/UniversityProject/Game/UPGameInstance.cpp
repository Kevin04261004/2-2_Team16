// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UPGameInstance.h"

#include "Blueprint/UserWidget.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Kismet/GameplayStatics.h"
#include "Player/UPPlayerController.h"

void UUPGameInstance::StartGame()
{
	if (CutSceneWidgetClass)
	{
		CutSceneWidgetInstance = CreateWidget<UUserWidget>(this, CutSceneWidgetClass);
		if (CutSceneWidgetInstance)
		{
			CutSceneWidgetInstance->AddToViewport();
		}
	}

	FString MapPath = TEXT("/Game/UniversityProject/Level/BossLevel");
	LoadPackageAsync(MapPath, FLoadPackageAsyncDelegate::CreateUObject(this, &UUPGameInstance::OnMapLoaded));
}

void UUPGameInstance::OnCutSceneFinished()
{
	bIsCutsceneFinished = true;
	TryOpenLevel(LoadedPackageName);
}

void UUPGameInstance::TryOpenLevel(const FName& PackageName)
{
	if (bIsMapLoaded && bIsCutsceneFinished)
	{
		if (CutSceneWidgetInstance)
		{
			CutSceneWidgetInstance->RemoveFromParent();
		}

		UGameplayStatics::OpenLevel(GetWorld(), PackageName);
		AUPPlayerController* PlayerController = Cast<AUPPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (PlayerController)
		{
			PlayerController->ClientSetCameraFade(true, FColor::Black, FVector2D(1.0f, 0.0f), 1.0f, false, true);
		}
	}
}

void UUPGameInstance::OnMapLoaded(const FName& PackageName, UPackage* LoadedPackage, EAsyncLoadingResult::Type Result)
{
	LoadedPackageName = PackageName;
	if (Result == EAsyncLoadingResult::Succeeded)
	{
		// 맵 로딩이 완료되었지만 컷 만화가 아직 끝나지 않으면 대기
		bIsMapLoaded = true;
		TryOpenLevel(PackageName);
	}
	else
	{
		// Handle loading failure if needed
		UE_LOG(LogTemp, Error, TEXT("Failed to load level: %s"), *PackageName.ToString());
	}
}
