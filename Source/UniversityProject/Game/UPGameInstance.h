// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "UPGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class UNIVERSITYPROJECT_API UUPGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void StartGame();

	UFUNCTION(BlueprintCallable)
	void OnCutSceneFinished();

	void TryOpenLevel(const FName& PackageName);

	FName LoadedPackageName;

private:
	UPROPERTY()
	UUserWidget* CutSceneWidgetInstance;

	UPROPERTY(EditAnywhere, Category = "UI", Meta = (AllowPrivateAccess = true))
	TSubclassOf<UUserWidget> CutSceneWidgetClass;

	
	
	bool bIsMapLoaded = false;
	bool bIsCutsceneFinished = false;
	void OnMapLoaded(const FName& PackageName, UPackage* LoadedPackage, EAsyncLoadingResult::Type Result);
};
