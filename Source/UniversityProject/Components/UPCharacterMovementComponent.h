// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UPCharacterStatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UPCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class UNIVERSITYPROJECT_API UUPCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UUPCharacterMovementComponent();

	void Initialize();
	
	FORCEINLINE bool GetIsSprinting() const { return bIsSprinting; }
	void SetIsSprinting(bool isSprinting);
	void SetCharacterStat(UUPCharacterStatComponent* InStatComponent);
	void Move(FVector2D MovementVector);
protected:
	bool bIsSprinting;
	UUPCharacterStatComponent* StatComponent;
	
	FTimerHandle SpeedChangeTimerHandle;
	float DesiredSpeed;
	float SpeedChangeRate = 10.0f; // 속도가 변화하는 비율

	void UpdateSpeed();

protected:
	TObjectPtr<class AUPCharacterBase> OwningCharacter;
};
