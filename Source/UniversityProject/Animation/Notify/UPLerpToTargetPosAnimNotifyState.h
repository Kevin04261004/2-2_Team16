// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "UPLerpToTargetPosAnimNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class UNIVERSITYPROJECT_API UUPLerpToTargetPosAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category="Init")
	FVector TargetLocalLocation;
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float InTotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lerp")
	UCurveFloat* LerpCurve;
private:
	FVector StartLocation; // 캐릭터의 초기 위치
	FVector TargetLocation;
	float StartTime;
	float TotalDuration;
};
