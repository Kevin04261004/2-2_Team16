// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AutoTargetingComponent.generated.h"

UENUM(BlueprintType)
enum class EAutoTargetingMode : uint8
{
	ATM_Nearest   UMETA(DisplayName = "Nearest Target"),
	ATM_Weakest   UMETA(DisplayName = "Weakest Target"),
	ATM_Strongest UMETA(DisplayName = "Strongest Target"),
	ATM_Farthest  UMETA(DisplayName = "Farthest Target")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNIVERSITYPROJECT_API UAutoTargetingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAutoTargetingComponent();
	
	AActor* FindDamageableTargetOrNull(const FVector& center, EAutoTargetingMode TargetingMode);
	void RotateToTarget(FVector targetLocation);
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Init", meta = (AllowPrivateAccess = "true"))
	float TargetSearchRadius;
private:
	static AActor* FindNearestTarget(const FVector& center, TArray<AActor*> targets);
	static AActor* FindFarthestTarget(const FVector& center, TArray<AActor*> targets);
	static AActor* FindStrongestTarget(const FVector& center, TArray<AActor*> targets);
	static AActor* FindWeakestTarget(const FVector& center, TArray<AActor*> targets);
	float TurnSpeed;
	FTimerHandle CameraLerpTimerHandle;
	FVector TargetLocation;
	FRotator LookAtRotation;
};
