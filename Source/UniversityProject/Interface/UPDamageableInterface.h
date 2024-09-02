// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UPDamagableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UUPDamagableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UNIVERSITYPROJECT_API IUPDamagableInterface
{
	GENERATED_BODY()

public:
	virtual float UPTakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) = 0;
};
