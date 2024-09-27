// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UPSkillData.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class UNIVERSITYPROJECT_API UUPSkillData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UUPSkillData();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	TObjectPtr<class UAnimMontage> SkillAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
	float AnimationSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Info")
	float CoolTime;

public:
	// 기획자가 블루프린트에서 커스터마이징 가능한 함수
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Damage Calculation")
	float CustomDamageCalculation(AActor* Character) const;
	virtual float CustomDamageCalculation_Implementation(AActor* Character) const;

protected:
	// 수식을 계산하는 내부 함수
	UFUNCTION(BlueprintCallable, Category="Damage Calculation")
	float CalculateDamage(float BaseAttackPower) const;
	
private:
	// 캐릭터의 공격력을 파라미터로 전달받아 데미지를 계산
	float GetCharacterAttackPower(AActor* Character) const;
};
