// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UPDamageableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UUPDamageableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UNIVERSITYPROJECT_API IUPDamageableInterface
{
	GENERATED_BODY()

public:
	virtual float UPTakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) = 0;
	virtual UParticleSystem* GetHitEffect() = 0;
	virtual USoundBase* GetHitSound() = 0;
};
