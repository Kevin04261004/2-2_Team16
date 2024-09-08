// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponSwingTrajectoryData.generated.h"

// 각 프레임에서 무기의 위치를 저장할 구조체
USTRUCT(BlueprintType)
struct FWeaponSwingFrameData
{
	GENERATED_BODY()

	// 휘두르기 전 포지션
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector StartPosition;

	// 휘두르기 후 포지션
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector EndPosition;
};

UCLASS()
class UNIVERSITYPROJECT_API AWeaponSwingTrajectoryData : public AActor
{
	GENERATED_BODY()

public:
	// 생성자
	AWeaponSwingTrajectoryData();

	// 애니메이션 프레임의 위치 데이터를 저장
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SwingData")
	TArray<FWeaponSwingFrameData> SwingFrameData;
};
