// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UPDashableStateInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UUPDashableStateInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UNIVERSITYPROJECT_API IUPDashableStateInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void TryDash() = 0;
};
