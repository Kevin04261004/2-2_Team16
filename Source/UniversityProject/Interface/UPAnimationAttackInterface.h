// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UPAnimationAttackInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UUPAnimationAttackInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UNIVERSITYPROJECT_API IUPAnimationAttackInterface
{
	GENERATED_BODY()

public:
	virtual void AttackHitCheck() = 0;
};
