// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/UPSkillManager.h"

#include "AutoTargetingComponent.h"
#include "UPCharacterStatComponent.h"
#include "Character/UPCharacterBase.h"
#include "Character/Weapon/UPPlayerCharacterWeapon.h"

// Sets default values for this component's properties
UUPSkillManager::UUPSkillManager()
{
	NextSkillType = EPlayerSkillType::None;

	AutoTargetingComponent = CreateDefaultSubobject<UAutoTargetingComponent>(TEXT("AutoTargeting"));

	// Skill
	InitSkillMap();
	CurrentCombo = 0;
}

void UUPSkillManager::BeginPlay()
{
	Super::BeginPlay();
	
	CreateDefaultObjectSkill();
}

void UUPSkillManager::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsComboCheckTime)
	{
		// ComboCheck();
	}
}

void UUPSkillManager::ProcessAttackCommand()
{
	bIsAttackKeyInput = true;
	if (CurrentCombo == 0)
	{
		ComboActionBegin();
	}
}

void UUPSkillManager::Initialize(AUPWeaponBase* Weapon, float* CurAttackDamage)
{
	OwningCharacter = Cast<AUPPlayerCharacter>(GetOwner());
	PlayerWeapon = Cast<AUPPlayerCharacterWeapon>(Weapon);
	this->AttackDamagePtr = CurAttackDamage;
}

void UUPSkillManager::InitSkillMap()
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

void UUPSkillManager::CreateDefaultObjectSkill()
{
	for (TTuple<EPlayerSkillType, TSubclassOf<UUPSkillBase>> skillMapTuple : SkillMapInitializer)
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
			EPlayerSkillType Type = skillMapTuple.Key;
			SkillMap.Add(Type, NewSkillComponent);

			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Skill Map Created"));
		}
	}
}

void UUPSkillManager::UseSkill(EPlayerSkillType skillType)
{
	UUPSkillBase** skill_pp = SkillMap.Find(skillType);
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

void UUPSkillManager::ComboActionFinish()
{
	CurrentCombo = 0;
}

void UUPSkillManager::StartComboCheck()
{
	bCanInputCommend = true;
}

void UUPSkillManager::EndComboCheck()
{
	bCanInputCommend = false;
}

void UUPSkillManager::ComboCheck()
{
	if (!bIsAttackKeyInput)
	{
		return;
	}
	
	if (CurrentCombo == 0)
	{
		UseSkill(EPlayerSkillType::Attack01);
	}
	EndComboCheck();
	bIsAttackKeyInput = false;
}
