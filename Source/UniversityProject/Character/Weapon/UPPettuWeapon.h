// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Weapon/UPWeaponBase.h"
#include "Character/Enemy/UPPettuCharacter.h"
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

public:
	void CheckAttackRange();
	void CheckAttackSocket(FName SocketName, EPettuSkillType SkillType, USkeletalMeshComponent* MeshComp);

	UFUNCTION(BlueprintCallable, Category = "Collision")
	void SetCollision();
	
	
	
	virtual void ClearAttackedActors() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", Meta = (AllowPrivateAccess = "true"))
	float AttackRange = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", Meta = (AllowPrivateAccess = "true"))
	float AttackRadius = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision")
	class USphereComponent* SphereCollision;

	UFUNCTION()
	void OnWeaponOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
							  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, 
							  const FHitResult& SweepResult);
	
protected:
	UFUNCTION()
	void CheckCollision();
	virtual void AttackSuccess(FHitResult& result, class IUPDamageableInterface* Damageable) override;

	
};
