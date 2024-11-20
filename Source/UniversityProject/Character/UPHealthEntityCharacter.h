// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameData/UPCharacterStatData.h"
#include "GameFramework/Character.h"
#include "Interface/UPDamageableInterface.h"
#include "UPHealthEntityCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnTakeDamaged, float /* DamagedAmount */)

UCLASS()
class UNIVERSITYPROJECT_API AUPHealthEntityCharacter : public ACharacter, public IUPDamageableInterface
{
	GENERATED_BODY()

public:
	AUPHealthEntityCharacter(const FObjectInitializer& ObjectInitializer);

/* Attack Hit Section */
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Init, Meta = (AllowPrivateAccess = "true", Tooltip = "피격시 생성되는 파티클"))
	TObjectPtr<UNiagaraSystem> HitEffect;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Init, Meta = (AllowPrivateAccess = "true", Tooltip = "피격시 생성되는 사운드"))
	TObjectPtr<USoundBase> HitSound;

	float CurAttackDamage;
	
	virtual float UPTakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

// Attack Hit Section
public:
	FORCEINLINE virtual UNiagaraSystem* GetHitEffect() override { return HitEffect.Get(); }
	FORCEINLINE virtual USoundBase* GetHitSound() override { return HitSound.Get(); }
	FOnTakeDamaged OnTakeDamaged;

// Dead Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category =Init, Meta = (AllowPrivateAccess = "true", Tooltip = "캐릭터 죽음 애니메이션 몽타주"))
	TObjectPtr<class UAnimMontage> DeadMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=STAT)
	bool bIsDead;

	virtual void SetDead();
	void PlayDeadAnimation();
	UFUNCTION()
	virtual void DeadAnimEnd(UAnimMontage* Montage, bool bInterrupted);
	
public:
	FORCEINLINE bool IsDead() const { return bIsDead; }

// Stat Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true", Tooltip = "스텟 컴포넌트"))
	TObjectPtr<class UUPCharacterStatComponent> StatComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category =Init, Meta = (AllowPrivateAccess = "true", Tooltip = "게임 시작 시 초기화 될 캐릭터의 스텟"))
	TObjectPtr<UUPCharacterStatData> CharacterInitalizeStatData;

public:
	FORCEINLINE const UUPCharacterStatComponent* GetStat() const { return StatComponent.Get(); }
};
