// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/UPCharacterBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"

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
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AI, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBehaviorTree> BTree;

	virtual float UPTakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void SetDead() override;

public:
	TObjectPtr<class UBehaviorTree> GetBehaviorTree() const { return BTree; }
	
private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BossStat, Meta = (AllowPrivateAccess = "true"))
	float MaxPatternActivateTime; // 패턴이 재발동 되기까지 걸리는 최대 시간 (1000당 1초)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BossStat, Meta = (AllowPrivateAccess = "true"))
	float MaxStunStack;			  // 보스가 스턴 상태가 되기 위한 스턴치 누적 최대값

	///////////// 상수 //////////////
	float MaxComboCount;
	float BaseComboFrameRate;
	float LastComboFrameRate;

	///////////// 변수 //////////////
	float CurrentHp;
	float CurrentPatternActivateTime;
	float DistanceFromPlayer;
	float CurrentStunStack;
	float DamageReceived;
	PettuStatus hasStatus;

	
	
};
