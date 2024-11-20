// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/UPMonsterBase.h"
#include "Skill/UPPettuSkillData.h"
#include "Skill/UPSkillBase.h"


AUPMonsterBase::AUPMonsterBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UUPCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	InitSkillMap();
	
}

void AUPMonsterBase::BeginPlay()
{
	Super::BeginPlay();
	CreateDefaultObjectSkill();
	MovementComponent->SetMovementMode(MOVE_Walking);
}

void AUPMonsterBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AUPMonsterBase::InitSkillMap()
{
	const UEnum* EnumPtr = StaticEnum<EPettuSkillType>();
	if (!EnumPtr)
	{
		return;
	}
	
	for (int32 i = 0; i < EnumPtr->GetMaxEnumValue(); ++i)  // GetMaxEnumValue()를 사용
	{
		EPettuSkillType EnumValue = static_cast<EPettuSkillType>(EnumPtr->GetValueByIndex(i));  // GetValueByIndex() 사용
		SkillMapInitializer.Add(EnumValue, nullptr);
	}
}

void AUPMonsterBase::CreateDefaultObjectSkill()
{
	for (auto skillMapTuple : SkillMapInitializer)
	{
		if (skillMapTuple.Value == nullptr)
		{
			continue;
		}
		
		if (UUPSkillBase* NewSkillComponent = NewObject<UUPSkillBase>(this, skillMapTuple.Value))
		{
			// 컴포넌트를 월드에 등록합니다.
			NewSkillComponent->RegisterComponent();

			// SkillMap에 새로 생성된 컴포넌트를 추가합니다.
			EPettuSkillType Type = skillMapTuple.Key;
			SkillMap.Add(Type, NewSkillComponent);

			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Skill Map Created"));
		}
	}
}

void AUPMonsterBase::SkillAttack(EPettuSkillType SkillType)
{
	UUPSkillBase** Skill = SkillMap.Find(SkillType);
	CurrentSkillData = Cast<UUPPettuSkillData>((*Skill)->GetSkillData());
	(*Skill)->TryActivateSkill(nullptr);
	CurAttackDamage = (*Skill)->GetSkillAttackDamage();
}
