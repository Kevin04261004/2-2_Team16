// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Interface/UPGameInterface.h"
#include "UPGameMode.generated.h"

/**
 * 
 */
UCLASS()
class UNIVERSITYPROJECT_API AUPGameMode : public AGameModeBase, public IUPGameInterface
{
	GENERATED_BODY()

public:
	AUPGameMode();

	virtual void OnPlayerDead() override;
	virtual bool IsGameCleared() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Game)
	uint8 bIsCleared : 1;
};
