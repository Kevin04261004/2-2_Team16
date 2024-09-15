// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UPPostProcessManager.generated.h"

UENUM()
enum EPostProcessMaterialType
{
	Blur = 0,
	SpeedLine = 1,
	
};

/**
 * 
 */
UCLASS()
class UNIVERSITYPROJECT_API UUPPostProcessManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	void TogglePostProcessMaterial(EPostProcessMaterialType MaterialType, bool bEnable = true, float activeTime = -1.f);
private:
	APostProcessVolume* PostProcessVolume;

	void FindPostProcessVolume();
	FTimerHandle PostProcessTimer;
};
