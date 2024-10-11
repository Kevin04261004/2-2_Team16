// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UPWeaponBase.h"
#include "GameFramework/Actor.h"
#include "Interface/UPResetAttackedActorList.h"
#include "UPPlayerCharacterWeapon.generated.h"

UCLASS()
class UNIVERSITYPROJECT_API AUPPlayerCharacterWeapon : public AUPWeaponBase
{
	GENERATED_BODY()

public:
	AUPPlayerCharacterWeapon();

public:
	// 애니메이션에서 매 프레임마다 호출하는 Notify함수
	void CheckAttackRange();
	
	// 콤보 공격이 종료되었을 때 호출되는 Notify함수
	virtual void ClearAttackedActors() override;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Weapon")
	TArray<FName> CollisionSocketNameArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Weapon")
	TArray<FVector> BeforeSocketLocationArray;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Weapon")
	TArray<FVector> SocketLocationArray;
	
	// Socket위치를 통해 콜리전 체크.
	void CheckCollisionSockets();
	virtual void AttackSuccess(FHitResult& result, class IUPDamageableInterface* Damageable) override;
/* Time Stop Section */
protected:

	// TimeManager에 접근하여 시간 멈춤 표현하기
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Time")
	float StopTimeVolume = 0.01f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Time")
	float StopTimeDuration = 0.1f;

	FTimerHandle GlobalTimeTimerHandler;
	void ResetTimeDilation();
};
