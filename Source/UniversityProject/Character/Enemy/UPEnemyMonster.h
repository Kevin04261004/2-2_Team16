// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UPMonsterBase.h"
#include "Interface/UPAnimationAttackCheckInterface.h"
#include "Components/TimelineComponent.h"
#include "UPEnemyMonster.generated.h"

UCLASS()
class UNIVERSITYPROJECT_API AUPEnemyMonster : public AUPMonsterBase, public IUPAnimationAttackCheckInterface 
{
	GENERATED_BODY()
public:
	AUPEnemyMonster(const FObjectInitializer& ObjectInitializer);
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

protected:
	virtual float UPTakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

/* Dead Section */
	virtual void SetDead() override;

	virtual void DeadAnimEnd(UAnimMontage* Montage, bool bInterrupted) override;

/* Pattern Section */
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pattern, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> PatternMontage;
	
	UFUNCTION()
	void PlayPatternMontage(UAnimMontage* Montage);

	UFUNCTION()
	void PatternMontageEnd(UAnimMontage* Montage, bool bInterrupted);
	
/* Attack Section */
public:
	virtual void AttackHitCheck() override;
	void AttackHitCheck(bool bIsAttached, FName SocketName, USkeletalMeshComponent* MeshComp,
		float AttackRange, float Amount, FVector CollisionLocation);

/* Knock Back Section */
public:
	UFUNCTION()
	void KnockbackProgress(float Value);
	UFUNCTION()
	void KnockbackFinished();
	
	bool IsKnockbackActive() const { return bIsKnockbackActive; }

	UPROPERTY(EditAnywhere, Category = "Knockback")
	float KnockbackStrength = 500.0f;

	UPROPERTY(EditAnywhere, Category = "Knockback")
	UAnimMontage* KnockbackMontage;

private:
	UPROPERTY()
	UTimelineComponent* KnockbackTimeline;

	UPROPERTY(EditAnywhere, Category = "Knockback")
	UCurveFloat* KnockbackCurve;

	FVector KnockbackStartLocation;
	FVector KnockbackEndLocation;
	
	bool bIsKnockbackActive = false;
};
