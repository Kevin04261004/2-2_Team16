// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameData/UPCharacterStat.h"
#include "UPCharacterStatComponent.generated.h"

// Delegate Section
DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangeDelegate, float /* CurrentHp */);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnStatChangedDelegate, const FUPCharacterStat& /* BaseStat */, const FUPCharacterStat& /* ModifierStat */);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNIVERSITYPROJECT_API UUPCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

// Init Section
public:
	UUPCharacterStatComponent();

// Init Section
protected:
	virtual void InitializeComponent() override;

// Stat Section
public:
	FOnHpZeroDelegate OnHpZero;
	FOnHpChangeDelegate OnHpChanged;
	FOnStatChangedDelegate OnStatChanged;

	void SetLevelStat(int32 InNewLevel);
	FORCEINLINE float GetCurrentLevel() const { return CurrentLevel; }
	FORCEINLINE void AddBaseStat(const FUPCharacterStat& InAddStat)
	{
		BaseStat = BaseStat + InAddStat;
		OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat());
	}
	FORCEINLINE void SetBaseStat(const FUPCharacterStat& InModifierStat)
	{
		BaseStat = InModifierStat;
		OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat());
	}
	FORCEINLINE void SetModifierStat(const FUPCharacterStat& InModifierStat)
	{
		ModifierStat = InModifierStat;
		OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat());
	}

	FORCEINLINE const FUPCharacterStat& GetBaseStat() const { return BaseStat; }
	FORCEINLINE const FUPCharacterStat& GetModifierStat() const { return ModifierStat; }
	FORCEINLINE FUPCharacterStat GetTotalStat() const { return BaseStat + ModifierStat; }
	FORCEINLINE float GetCurrentHp() const { return CurrentHp; }
	FORCEINLINE void HealHp(float InHealAmount)
	{
		CurrentHp = FMath::Clamp(CurrentHp + InHealAmount, 0, GetTotalStat().MaxHp);
		OnHpChanged.Broadcast(CurrentHp);
	}
	FORCEINLINE float GetAttackRadius() const { return AttackRadius; }
	float ApplyDamage(float InDamage);

// Data Section
protected:
	void SetHp(float NewHp);

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentLevel;

	UPROPERTY(VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float AttackRadius;
	
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FUPCharacterStat BaseStat;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FUPCharacterStat ModifierStat;
};
