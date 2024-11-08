// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UPKnockBackableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UUPKnockBackableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UNIVERSITYPROJECT_API IUPKnockBackableInterface
{
	GENERATED_BODY()

public:
	virtual void KnockBack(float power) = 0;
};
