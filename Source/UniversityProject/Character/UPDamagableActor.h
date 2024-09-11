// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/UPDamageableInterface.h"
#include "GameData/UPCharacterStatData.h"
#include "UPDamagableActor.generated.h"

UCLASS()
class UNIVERSITYPROJECT_API AUPDamagableActor : public AActor, public IUPDamageableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUPDamagableActor();
	
	virtual void PostInitializeComponents() override;
// Damage Section
protected:
	virtual float UPTakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void SetDead();
// Stat Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UUPCharacterStatComponent> Stat;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UUPCharacterStatData> CharacterInitalizeStatData;

// Stat Section
public:
	FORCEINLINE const UUPCharacterStatComponent* GetStat() const { return Stat.Get(); }
	void ApplyStat(const FUPCharacterStat& BaseStat, const FUPCharacterStat& ModifierStat);
};
