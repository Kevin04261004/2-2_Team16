// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UPTimeManager.generated.h"

/**
 * 
 */
UCLASS()
class UNIVERSITYPROJECT_API UUPTimeManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	void WorldTimeUp();
	void WorldTimeDown();
	void WorldTimeReset();

protected:
	float timeScale = 1.0f;

	void SetGlobalTimeDilation(float NewTimeScale);
};
