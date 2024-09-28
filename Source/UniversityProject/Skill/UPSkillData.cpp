// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/UPSkillData.h"

// 기본으로 클래스가 생성될 때 초기화 값.
UUPSkillData::UUPSkillData() : AnimationSpeed(1.0f), CoolTime(0.0f), StunStack(1.0f), bCanStopSkill(false), bIsAutoTargetingSkill(true), SkillAdditionalTime(0.0f)
{
	SkillAnimation = nullptr;
}

float UUPSkillData::CustomDamageCalculation_Implementation(float BaseAttackDamage) const
{
	// 커스터마이징을 하지 않았을 때, 그냥 기본 공격력을 스킬 데미지로 리턴.
	// CalculateDamage 함수에 캐릭터의 공격력 전달
	return BaseAttackDamage;
}
