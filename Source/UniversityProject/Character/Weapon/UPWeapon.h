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
	
protected:
	// 메쉬
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	USkeletalMeshComponent* WeaponMesh;
};
