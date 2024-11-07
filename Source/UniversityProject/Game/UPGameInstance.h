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

private:
	UPROPERTY()
	UUserWidget* LoadingWidgetInstance;

	UPROPERTY(EditAnywhere, Category = "UI", Meta = (AllowPrivateAccess = true))
	TSubclassOf<UUserWidget> LoadingWidgetClass;

	void OnMapLoaded(const FName& PackageName, UPackage* LoadedPackage, EAsyncLoadingResult::Type Result);
};
