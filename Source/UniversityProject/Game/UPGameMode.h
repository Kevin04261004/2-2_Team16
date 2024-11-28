// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Interface/UPGameInterface.h"
#include "Stage/UPStageManager.h"
#include "UI/UPHudWidget.h"
#include "UI/UPSettingWidget.h"
#include "UPGameMode.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class UNIVERSITYPROJECT_API AUPGameMode : public AGameModeBase, public IUPGameInterface
{
	GENERATED_BODY()

public:
	AUPGameMode();

	virtual void OnPlayerDead() override;
	
	UFUNCTION(BlueprintCallable)
	virtual void OnGameClear() override;

	virtual void OnTutorialClear();

	virtual bool IsGameCleared() override;
	FORCEINLINE virtual bool IsTutorialStage() { return bIsTutorialCleared == true; }
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Game)
	uint8 bIsCleared : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Game)
	uint8 bIsTutorialCleared : 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Game)
	bool bIsGameOver = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Game)
	TSubclassOf<UUserWidget> HudWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Game)
	TObjectPtr<UUPHudWidget> HudWidgetObject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Game)
	TSubclassOf<UUPUserWidget> SettingWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Game)
	TObjectPtr<UUPSettingWidget> SettingWidgetObject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Game)
	TSubclassOf<UUPUserWidget> SkipTutorial;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Game)
	TObjectPtr<UUPSkipTutorialWidget> SkipTutorialObject;
	
	UPROPERTY()
	TSubclassOf<AUPStageManager> StageManagerClass;
	UPROPERTY()
	TObjectPtr<AUPStageManager> StageManager;

};
