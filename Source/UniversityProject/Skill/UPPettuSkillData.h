// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/UPSkillData.h"
#include "UPPettuSkillData.generated.h"

/**
 * 
 */
UCLASS()
class UNIVERSITYPROJECT_API UUPPettuSkillData : public UUPSkillData
{
	GENERATED_BODY()
public:
	UUPPettuSkillData();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Data", meta = (Tooltip = "스킬 범위"))
	float SkillRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Data", meta = (Tooltip = "충돌체 위치"))
	FVector CollisionLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Data", meta = (Tooltip = "위치값"))
	float Amount;
public:
	FORCEINLINE float GetSkillRange() const { return SkillRange; }
	FORCEINLINE FVector GetCollisionLocation() const { return CollisionLocation; }
	FORCEINLINE float GetAmount() const { return Amount; }
};
