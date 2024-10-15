// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Interface/UPGameInterface.h"
#include "UI/UPHudWidget.h"
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
	virtual bool IsGameCleared() override;
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Game)
	uint8 bIsCleared : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Game)
	TSubclassOf<UUserWidget> HudWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Game)
	TObjectPtr<UUPHudWidget> HudWidgetObject;
};
