// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UPPlayerController.generated.h"

UENUM(BlueprintType)
enum class EInputMode : uint8
{
	Game,
	UI,
};

/**
 * 
 */
UCLASS()
class UNIVERSITYPROJECT_API AUPPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AUPPlayerController();

	UFUNCTION(BlueprintImplementableEvent, Category = Game, Meta = (DisplayName = "OnGameOverCpp"))
	void K2_OnGameOver();
	void GameOver();

	UFUNCTION(BlueprintImplementableEvent, Category = Game, Meta = (DisplayName = "OnGameClearCpp"))
	void K2_OnGameClear();
	void GameClear();
protected:
	virtual void BeginPlay() override;

public:
	void SetUIMode();
	void SetGameMode();
	FORCEINLINE EInputMode GetInputMode() const { return CurInputMode; }
	
protected:
	EInputMode CurInputMode;

protected:
	TObjectPtr<class UUPSettingWidget> SettingWidgetObject;
};
