// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UPMonsterBase.h"
#include "Character/UPCharacterBase.h"
#include "Interface/UPAnimationAttackCheckInterface.h"
#include "Interface/UPCharacterHUDInterface.h"
#include "Interface/UPPettuPunchTrailInterface.h"
#include "UPPettuCharacter.generated.h"

UCLASS()
class UNIVERSITYPROJECT_API AUPPettuCharacter : public AUPMonsterBase, public IUPAnimationAttackCheckInterface, public IUPCharacterHUDInterface, public IUPPettuPunchTrailInterface
{
	GENERATED_BODY()
public:
	AUPPettuCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;
	
protected:
	virtual float UPTakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:
	TObjectPtr<AUPPlayerCharacter> PlayerCharacter;
	
/* State Section */
public:
	
/* Dead Section */
	virtual void SetDead() override;

	virtual void DeadAnimEnd(UAnimMontage* Montage, bool bInterrupted) override;

/* Stun Section */
	FORCEINLINE bool IsPettuStun() const { return IsStun(); }
	virtual void SetStun() override;
	UFUNCTION()
	void StunEnd(UAnimMontage* Montage, bool bInterrupted);
	

/* Stiffen Section */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Meta = (AllowPrivateAccess = "true"))
	bool bIsStiffen;
	void SetStiffen();
	UFUNCTION()
	void StiffenEnd(UAnimMontage* Montage, bool bInterrupted);
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Init, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> StiffenMontage;
	void PlayStiffenAnimation();
	
private:
	UFUNCTION()
	void TestFunc();
	FTimerHandle TestHandle;
	
/* Pattern Section */
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pattern, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> PatternMontage;
	
	UFUNCTION()
	void PlayPatternMontage(UAnimMontage* Montage);

	UFUNCTION()
	void PatternMontageEnd(UAnimMontage* Montage, bool bInterrupted);

	float GetStunStack() const { return StatComponent->GetCurrentStunStack(); }
	
/* Attack Section */
public:
	virtual void AttackHitCheck() override;
	void AttackHitCheck(bool bIsAttached, FName SocketName, USkeletalMeshComponent* MeshComp);
	void AttackHitCheck(bool bIsAttached, FName SocketName, USkeletalMeshComponent* MeshComp,
		float AttackRange, float Amount, FVector CollisionLocation);

	/* UI Section */
protected:
	virtual void SetupHUDWidget(UUPHudWidget* InHUDWidget) override;
	
	UFUNCTION()
	void StunCheck(float Hp);

/* Punch */
protected:
	virtual void PunchTrailOn(EPunchTrailType type) override;
	virtual void PunchTrailOff(EPunchTrailType type) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effects")
	TObjectPtr<UNiagaraComponent> LeftHandEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effects")
	TObjectPtr<UNiagaraComponent> RightHandEffect;
	
/* Effects */
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Init", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraSystem> NiagaraSystem;
	
};
