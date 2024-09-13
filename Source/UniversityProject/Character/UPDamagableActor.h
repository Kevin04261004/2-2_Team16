// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/UPDamageableInterface.h"
#include "GameData/UPCharacterStatData.h"
#include "Interface/UPWidgetInterface.h"
#include "UPDamagableActor.generated.h"

UCLASS()
class UNIVERSITYPROJECT_API AUPDamagableActor : public AActor, public IUPDamageableInterface, public IUPWidgetInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUPDamagableActor();
	
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
// Damage Section
protected:
	virtual float UPTakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void SetDead();
	
// Attack Hit Section
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Effect, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UParticleSystem> HitEffect;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Effect, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundBase> HitSound;
	FORCEINLINE virtual UParticleSystem* GetHitEffect() override { return HitEffect.Get(); }
	FORCEINLINE virtual USoundBase* GetHitSound() override { return HitSound.Get(); }
	
// Stat Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UUPCharacterStatComponent> Stat;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UUPCharacterStatData> CharacterInitalizeStatData;

// Stat Section
public:
	FORCEINLINE const UUPCharacterStatComponent* GetStat() const { return Stat.Get(); }

	// UI Widget Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UUPWidgetComponent> HpBar;
	
	virtual void SetupCharacterWidget(class UUPUserWidget* InUserWidget) override;
};
