// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UPResetAttackedActorList.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UUPResetAttackedActorList : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UNIVERSITYPROJECT_API IUPResetAttackedActorList
{
	GENERATED_BODY()

public:
	virtual void ResetAttackedActorList() = 0;
};
