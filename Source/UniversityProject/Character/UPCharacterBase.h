// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameData/UPCharacterStatData.h"
#include "GameData/UPCharacterStat.h"
#include "GameFramework/Character.h"
#include "Interface/UPDamageableInterface.h"
#include "UPCharacterBase.generated.h"

UCLASS()
class UNIVERSITYPROJECT_API AUPCharacterBase : public ACharacter, public IUPDamageableInterface
{
	GENERATED_BODY()

// Init Section
public:
	AUPCharacterBase();
	
	virtual void PostInitializeComponents() override;
	
// ComboAction Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ComboAttack, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UUPComboAttackComponent> ComboAttack;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ComboAttack, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> Weapon;

// ComboAction Section
public:
	virtual void NotifyComboActionEnd();
	
// Attack Hit Section
protected:
	virtual float UPTakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
// Dead Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

	virtual void SetDead();
	void PlayDeadAnimation();

	float DeadEventDelayTime = 5.0f;

// Stat Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UUPCharacterStatComponent> Stat;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UUPCharacterStatData> CharacterInitalizeStatData;
// Stat Section
public:
	int32 GetLevel();
	void SetLevel(int32 InNewLevel);
	FORCEINLINE const UUPCharacterStatComponent* GetStat() const { return Stat.Get(); }
	void ApplyStat(const FUPCharacterStat& BaseStat, const FUPCharacterStat& ModifierStat);
};
