// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Weapon/UPWeaponBase.h"
#include "Character/Enemy/UPPettuCharacter.h"
#include "Components/SphereComponent.h"
#include "Skill/UPPettuSkillData.h"
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
	void CheckAttackRange(float AttackRange, float Amount, FVector CollisionLocation);
	void CheckAttackSocket(FName SocketName, UUPPettuSkillData* SkillData, USkeletalMeshComponent* MeshComp);
	void CheckAttackRange(FName SocketName, USkeletalMeshComponent* MeshComp,
		float AttackRange, FVector CollisionLocation);

	UFUNCTION(BlueprintCallable, Category = "Collision")
	void SetCollision(bool bIsCollision);
	
	virtual void ClearAttackedActors() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", Meta = (AllowPrivateAccess = "true"))
	float AttackRadius = 500.0f;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision")
	TObjectPtr<USphereComponent> SphereCollision;

	UFUNCTION()
	void OnWeaponOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
							  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, 
							  const FHitResult& SweepResult);
	
protected:
	UFUNCTION()
	void CheckCollision(FVector Start, FVector End);
	virtual void AttackSuccess(FHitResult& result, class IUPDamageableInterface* Damageable) override;

	
};
