
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UPPostProcessManager.generated.h"

UENUM()
enum EPostProcessMaterialType
{
	Blur = 0,
	SpeedLine = 1,
	EdgeFadeDesaturation = 2,
	Blode = 3,
};

/**
 * 
 */
UCLASS()
class UNIVERSITYPROJECT_API UUPPostProcessManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual auto Initialize(FSubsystemCollectionBase& Collection) -> void override;
	
	void TogglePostProcessMaterial(EPostProcessMaterialType MaterialType, bool bEnable = true, float activeTime = -1.f);
private:
	APostProcessVolume* PostProcessVolume;

	void FindPostProcessVolume();
	FTimerHandle PostProcessTimer;
};
