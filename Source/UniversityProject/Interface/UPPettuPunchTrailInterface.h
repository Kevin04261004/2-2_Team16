// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/Notify/UPPettuPunchTrailAnimNotifyState.h"
#include "UObject/Interface.h"
#include "UPPettuPunchTrailInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UUPPettuPunchTrailInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UNIVERSITYPROJECT_API IUPPettuPunchTrailInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void PunchTrailOn(EPunchTrailType type) = 0;
	virtual void PunchTrailOff(EPunchTrailType type) = 0;
};
