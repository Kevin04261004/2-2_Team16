// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/UPPlayerCharacter.h"
#include "Character/Weapon/UPPlayerCharacterWeapon.h"
#include "Components/ActorComponent.h"
#include "UPSkillManager.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnComboStepEnd);
DECLARE_MULTICAST_DELEGATE(FOnComboAttackFinish);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNIVERSITYPROJECT_API UUPSkillManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	UUPSkillManager();

	void ProcessAttackCommand();

	void Initialize(AUPWeaponBase* Weapon, float* CurAttackDamage);
// Delegate Section
public:
	FOnComboAttackFinish OnComboAttackFinish;
	FOnComboStepEnd OnComboStepEnd;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Init)
	TMap<EPlayerSkillType, TSubclassOf<UUPSkillBase>> SkillMapInitializer;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Init)
	TMap<EPlayerSkillType, UUPSkillBase*> SkillMap;
	void InitSkillMap();
	void CreateDefaultObjectSkill();
	void UseSkill(EPlayerSkillType skillType);

/* Auto Targeting */
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AutoTargeting, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UAutoTargetingComponent> AutoTargetingComponent;

	
protected:
	virtual void BeginPlay() override;
	
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void ComboActionFinish();
	void StartComboCheck();
	void EndComboCheck();
	void ComboCheck();

	int32 CurrentCombo = 0;
	EPlayerSkillType NextSkillType;
	bool bCanInputCommend;
	bool bIsComboCheckTime;
	bool bIsAttackKeyInput;
	
	TObjectPtr<AUPPlayerCharacter> OwningCharacter;

/* Owner's Reference Values */
protected:
	float* AttackDamagePtr;
	TObjectPtr<AUPPlayerCharacterWeapon> PlayerWeapon;
};
