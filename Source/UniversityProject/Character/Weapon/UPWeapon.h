// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UPWeapon.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnWeaponHit, FHitResult& /* Result */);

UCLASS()
class UNIVERSITYPROJECT_API AUPWeapon : public AActor
{
	GENERATED_BODY()

public:
	AUPWeapon();

	void Tick(float DeltaSeconds) override;
	// 애니메이션에서 매 프레임마다 호출하는 Notify함수
	void CheckAttackRange();
	
	// 콤보 공격이 종료되었을 때 호출되는 Notify함수
	void ComboStepEnd();

	// 상대와 충돌함.
	void Attack(FHitResult& result);

	FOnWeaponHit OnWeaponHit;
protected:
	// 메쉬
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Weapon")
	TArray<FName> CollisionSocketNameArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Weapon")
	TArray<FVector> BeforeSocketLocationArray;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Weapon")
	TArray<FVector> SocketLocationArray;

	/* 상대방에 Effect가 없으면 실행되는 기본 이펙트 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effects")
	UParticleSystem* BaseHitEffect;

	/* 상대방에 Effect가 없으면 실행되는 기본 이펙트 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sounds")
	USoundBase* BaseHitSound;
	
	TArray<AActor*> AttackedActors;
	
	// Socket위치를 통해 콜리전 체크.
	void CheckCollisionSockets();

/* Time Stop Section */
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Time")
	float StopTimeVolume = 0.01f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Time")
	float StopTimeDuration = 0.1f;
	float RealTimeAtStart;
	bool bIsTimeStopped;
	
	void ResetTimeDilation();
};
