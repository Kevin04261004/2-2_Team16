// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponSwingTrajectoryData.h"
#include "GameFramework/Actor.h"
#include "UPWeapon.generated.h"

UCLASS()
class UNIVERSITYPROJECT_API AUPWeapon : public AActor
{
	GENERATED_BODY()

public:
	AUPWeapon();

	// 궤적 데이터를 저장할 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	AWeaponSwingTrajectoryData* SwingTrajectoryData;
	
	// 공격 중 프레임마다 호출되는 함수
	void PerformSwing(int32 CurrentFrameIndex);

	// 애니메이션에서 매 프레임마다 호출하는 Notify함수
	void NotifyAttackCheck();
protected:
	// 메쉬
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Weapon")
	TArray<FName> CollisionSocketNameArray;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Weapon")
	TArray<FVector> SocketLocationArray;

	// Socket위치를 통해 콜리전 체크.
	void CheckCollisionSockets();
};
