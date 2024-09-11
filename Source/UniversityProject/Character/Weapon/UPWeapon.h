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

	TArray<AActor*> AttackedActors;
	
	// Socket위치를 통해 콜리전 체크.
	void CheckCollisionSockets();
};
