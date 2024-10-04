// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/UPCharacterBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Skill/UPSkillBase.h"
#include "AI/UPPettuAIController.h"
#include "Interface/UPAnimationAttackCheckInterface.h"
#include "UPPettuCharacter.generated.h"

UENUM(BlueprintType)
enum class EPettuSkillType : uint8
{
	SmashAttack UMETA(DisplayName = "내려찍기"),
	SmashAttack2 UMETA(DisplayName = "내려찍기2"),
	SmashAttack3 UMETA(DisplayName = "내려찍기3"),
};

UCLASS()
class UNIVERSITYPROJECT_API AUPPettuCharacter : public AUPCharacterBase, public IUPAnimationAttackCheckInterface
{
	GENERATED_BODY()
public:
	AUPPettuCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;
	
protected:
	virtual float UPTakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
/* AI Section */
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AI, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBehaviorTree> BTree;
	
	TObjectPtr<class UBehaviorTree> GetBehaviorTree() const { return BTree; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class AAIController> PettuAIController;
	
/* State Section */
public:
	
/* Dead Section */
	FORCEINLINE bool IsPettuDead() const { return IsDead(); }
	virtual void SetDead() override;

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

	///////////// 상수 //////////////
	float MaxComboCount;
	float BaseComboFrameRate;
	float LastComboFrameRate; 

	///////////// 변수 //////////////
	float DamageReceived;

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

/* Skill Section */
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Init, Meta = (AllowPrivateAccess = true))
	TMap<EPettuSkillType, TSubclassOf<UUPSkillBase>> SkillMapInitializer;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Init, Meta = (AllowPrivateAccess = true))
	TMap<EPettuSkillType, UUPSkillBase*> SkillMap;
	void InitSkillMap();
	void CreateDefaultObjectSkill();
/* Attack Section */
protected:
	virtual void AttackHitCheck() override;
	
};
