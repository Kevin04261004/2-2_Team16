// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Player/UPSkillManagerComponent.h"
#include "Character/UPPlayerCharacter.h"
#include "Character/Weapon/UPPlayerCharacterWeapon.h"
#include "Components/AutoTargetingComponent.h"
#include "Skill/UPSkillBase.h"

UUPSkillManagerComponent::UUPSkillManagerComponent()
{
	AutoTargetingComponent = CreateDefaultSubobject<UAutoTargetingComponent>(TEXT("AutoTargeting"));
	InitSkillMap();
}

void UUPSkillManagerComponent::Initialize(AUPWeaponBase* Weapon, float* CurAttackDamage)
{
	OwningCharacter = Cast<AUPPlayerCharacter>(GetOwner());
	PlayerWeapon = Cast<AUPPlayerCharacterWeapon>(Weapon);
	this->AttackDamagePtr = CurAttackDamage;
}

void UUPSkillManagerComponent::InitSkillMap()
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EPlayerSkillType"), true);
	if (!EnumPtr)
	{
		return;
	}
	
	for (int32 i = 0; i < EnumPtr->NumEnums() - 1; ++i)
	{
		if (!EnumPtr->HasMetaData(TEXT("Hidden"), i))
		{
			EPlayerSkillType EnumValue = static_cast<EPlayerSkillType>(EnumPtr->GetValueByIndex(i));
			SkillMapInitializer.Add(EnumValue, nullptr);
		}
	}
}

void UUPSkillManagerComponent::CreateDefaultObjectSkill()
{
	for (TTuple<EPlayerSkillType, TSubclassOf<UUPSkillBase>> skillMapTuple : SkillMapInitializer)
	{
		if (skillMapTuple.Value == nullptr)
		{
			continue;
		}
		UUPSkillBase* NewSkillComponent = NewObject<UUPSkillBase>(this, skillMapTuple.Value);
		if (NewSkillComponent != nullptr)
		{
			// 컴포넌트를 월드에 등록합니다.
			NewSkillComponent->RegisterComponent();
			// SkillMap에 새로 생성된 컴포넌트를 추가합니다.
			EPlayerSkillType Type = skillMapTuple.Key;
			SkillMap.Add(Type, NewSkillComponent);
			check(OwningCharacter->MovementComponent != nullptr);
			check(OwningCharacter != nullptr);
			NewSkillComponent->Initialize(OwningCharacter->MovementComponent, OwningCharacter);
			
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Skill Map Created"));
		}
	}
}

void UUPSkillManagerComponent::UseSkill(EPlayerSkillType InSkillType)
{
	UUPSkillBase** skill_pp = SkillMap.Find(InSkillType);
	if (skill_pp == nullptr)
	{
		return;
	}
	UUPSkillBase* skill = *skill_pp;
	if (skill == nullptr)
	{
		return;
	}
	
	// 타겟팅, 오토타겟팅이면 상대를 바라보게!
	AActor* Target = nullptr;
	if (skill->CanUseSkill() && skill->GetSkillData()->IsAutoTargetingSkill())
	{
		Target = AutoTargetingComponent->FindDamageableTargetOrNull(OwningCharacter->GetActorLocation(), EAutoTargetingMode::ATM_Nearest);
		if (Target != nullptr)
		{
			FVector TargetLocation = Target->GetActorLocation();
			AutoTargetingComponent->RotateToTarget(TargetLocation);
		}
	}
	if (skill->CanUseSkill())
	{
		// TODO: 모듈화
		AUPPlayerCharacterWeapon* playerWeapon = Cast<AUPPlayerCharacterWeapon>(PlayerWeapon);
		playerWeapon->ComboStepEnd();
		*AttackDamagePtr = skill->GetSkillData()->GetSkillDamage(OwningCharacter->GetStat()->GetTotalStat().AttackDamage);
		skill->TryActivateSkill(Target);
	}
}
