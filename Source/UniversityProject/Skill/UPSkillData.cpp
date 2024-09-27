// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/UPSkillData.h"

#include "Character/UPCharacterBase.h"

UUPSkillData::UUPSkillData() : AnimationSpeed(1.0f), CoolTime(0.0f)
{
}

float UUPSkillData::CustomDamageCalculation_Implementation(AActor* Character) const
{
	// 캐릭터의 공격력을 얻어옴
	float CharacterAttackPower = GetCharacterAttackPower(Character);

	// CalculateDamage 함수에 캐릭터의 공격력 전달
	return CalculateDamage(CharacterAttackPower);
}

float UUPSkillData::CalculateDamage(float BaseAttackPower) const
{
	return BaseAttackPower;
}

float UUPSkillData::GetCharacterAttackPower(AActor* Character) const
{
	// 캐릭터가 UPCharacter 클래스라면, 공격력을 가져옴
	AUPCharacterBase* MyCharacter = Cast<AUPCharacterBase>(Character);
	if (MyCharacter)
	{
		return MyCharacter->GetStat()->GetTotalStat().AttackDamage;
	}

	// 캐릭터가 없거나 잘못된 타입이면 기본 값 반환
	return 0.0f;
}
