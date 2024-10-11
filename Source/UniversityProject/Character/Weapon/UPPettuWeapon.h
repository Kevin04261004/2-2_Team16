// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Weapon/UPWeaponBase.h"
#include "UPPettuWeapon.generated.h"

/**
 * 
 */
UCLASS()
class UNIVERSITYPROJECT_API AUPPettuWeapon : public AUPWeaponBase
{
	GENERATED_BODY()
public:
	AUPPettuWeapon();

protected:
	// Tick

public:
	void CheckAttackRange();
	virtual void ClearAttackedActors() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", Meta = (AllowPrivateAccess = "true"))
	float AttackRange = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", Meta = (AllowPrivateAccess = "true"))
	float AttackRadius = 500.0f;
	
protected:
	UFUNCTION()
	void CheckCollision();
	virtual void AttackSuccess(FHitResult& result, class IUPDamageableInterface* Damageable) override;
};
