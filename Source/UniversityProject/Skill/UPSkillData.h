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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Animation", meta = (Tooltip = "스킬 애니메이션"))
	TObjectPtr<class UAnimMontage> SkillAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Animation", meta = (Tooltip = "애니메이션이 몇 배속으로 실행되는지. 기본값은 1이다."))
	float AnimationSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Data", meta = (Tooltip = "스킬 쿨타임. 스킬에 쿨타임이 존재하지 않으면 0으로 고정한다. 대시 같은 애니메이션이 끝나야 사용이 가능한 스킬도 0으로 해주세요."))
	float CoolTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Data", meta = (Tooltip = "애니메이션이 끝나고, 몇초동안 추가적으로 스킬이 발동되는지 작성합니다. 기본값은 0입니다. 만약 0일시 애니메이션이 끝나면 스킬이 종료됩니다."))
	float SkillAdditionalTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Data", meta = (Tooltip = "상대방을 때릴 때 상대방에게 스턴 스텍을 먹인다. 스턴 스텍이 전부 까이면 스턴이 된다."))
	float StunStack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Data", meta = (Tooltip = "스킬 시전 중 스탑이 가능한가?"))
	bool bCanStopSkill;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Data", meta = (Tooltip = "오토 타겟팅이 가능한 스킬인가?"))
	bool bIsAutoTargetingSkill;
	
	// 기획자가 블루프린트에서 커스터마이징 가능한 함수
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Damage Calculation", meta = (Tooltip = "기본공격에서 추가 데미지에 대한 계산식을 작성한다."))
	float CustomDamageCalculation(float BaseAttackDamage) const;

	// 기획자가 커스터마이징을 하지 않으면 아래 함수(<커스텀 함수명>_Implementation)가 호출이 되어요!!
	virtual float CustomDamageCalculation_Implementation(float BaseAttackDamage) const;
public:
	FORCEINLINE float GetCoolTime() const { return CoolTime; }
	FORCEINLINE float GetStunStack() const { return StunStack; }
	FORCEINLINE float GetAnimationSpeed(float BaseAttackSpeed) const { return AnimationSpeed * BaseAttackSpeed; }
	FORCEINLINE UAnimMontage* GetSkillAnimation() const { return SkillAnimation.Get(); }
	FORCEINLINE float GetSkillDamage(float BaseAttackDamage) const { return CustomDamageCalculation(BaseAttackDamage); }
	FORCEINLINE float GetCoolDuration(float BaseAttackSpeed) const { return CoolTime + GetSkillDuration(BaseAttackSpeed); }
	FORCEINLINE float GetSkillDuration(float BaseAttackSpeed) const { return SkillAnimation->GetPlayLength() * (1.0f / GetAnimationSpeed(BaseAttackSpeed)) + SkillAdditionalTime; }
	FORCEINLINE bool CanStopSkill() const { return bCanStopSkill; }
	FORCEINLINE bool IsAutoTargetingSkill() const { return bIsAutoTargetingSkill; }
};
