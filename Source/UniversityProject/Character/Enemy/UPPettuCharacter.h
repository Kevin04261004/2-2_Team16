// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/UPCharacterBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AI/UPPettuAIController.h"
#include "UPPettuCharacter.generated.h"

UENUM(BlueprintType)
enum class PettuStatus : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Move UMETA(DisplayName = "Move"),
	Pattern_1 UMETA(DisplayName = "Pattern_1"),
	Pattern_2 UMETA(DisplayName = "Pattern_2"),
	Pattern_3 UMETA(DisplayName = "Pattern_3"),
	Stiffend UMETA(DisplayName = "Stiffend"),
	Stunned UMETA(DisplayName = "Stunned"),
	Dead UMETA(DisplayName = "Dead"),
};
UCLASS()
class UNIVERSITYPROJECT_API AUPPettuCharacter : public AUPCharacterBase
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
	void SetPettuDead();

/* Stun Section */
	FORCEINLINE bool IsPettuStun() const { return IsStun(); }
	virtual void SetStun() override;
	UFUNCTION()
	void StunEnd(UAnimMontage* Montage, bool bInterrupted);

/* Stiffen Section */
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
	float CurrentHp;
	float DamageReceived;
	PettuStatus hasStatus;

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
};
