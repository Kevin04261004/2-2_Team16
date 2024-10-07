// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/UPPlayerCharacter.h"
#include "Components/ActorComponent.h"
#include "Skill/UPSkillBase.h"
#include "UPSkillManagerComponent.generated.h"


enum class EPlayerSkillType : uint8;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNIVERSITYPROJECT_API UUPSkillManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UUPSkillManagerComponent();

	void Initialize(class AUPWeaponBase* Weapon, float* CurAttackDamage);
	void UseSkill(EPlayerSkillType InSkillType);
	FORCEINLINE UUPSkillBase* GetSkill(EPlayerSkillType InSkillType) { return SkillMap[InSkillType]; }
	FORCEINLINE bool CanUseSkill(EPlayerSkillType InSkillType) { return SkillMap[InSkillType]->CanUseSkill(); }
	void CreateDefaultObjectSkill();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Init)
	TMap<EPlayerSkillType, TSubclassOf<class UUPSkillBase>> SkillMapInitializer;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Init)
	TMap<EPlayerSkillType, class UUPSkillBase*> SkillMap;
	
	void InitSkillMap();

/* Auto Targeting */
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AutoTargeting, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UAutoTargetingComponent> AutoTargetingComponent;

/* Owner's Reference Values */
protected:
	float* AttackDamagePtr;
	AUPPlayerCharacter* OwningCharacter;
	TObjectPtr<class AUPPlayerCharacterWeapon> PlayerWeapon;
};
