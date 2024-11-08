// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/UPCharacterMovementComponent.h"
#include "GameData/UPCharacterStatData.h"
#include "GameData/UPCharacterStat.h"
#include "GameFramework/Character.h"
#include "Interface/UPDamageableInterface.h"
#include "Interface/UPKnockBackableInterface.h"
#include "UPCharacterBase.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnTakeDamaged, float /* DamagedAmount */)

UCLASS()
class UNIVERSITYPROJECT_API AUPCharacterBase : public ACharacter, public IUPDamageableInterface
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MovementComponent, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UUPCharacterMovementComponent> MovementComponent;
	
// Init Section1
public:
	AUPCharacterBase(const FObjectInitializer& ObjectInitializer);
	virtual void BeginPlay() override;
	
	virtual void PostInitializeComponents() override;
// Utils...
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Init, meta=(AllowPrivateAccess= "true", Tooltip = "캐릭터가 사용하는 애니메이션 클래스"))
	TObjectPtr<UClass> AnimInstanceClass;

// Utils...
public:
	bool TryCheckForwardCollision(float InLineTraceDistance, FHitResult& OutHit, FVector& OutActorLocation);

	FORCEINLINE UUPCharacterStatComponent* GetStatComponent() const { return StatComponent.Get(); }
// Weapon Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category =Init, Meta = (AllowPrivateAccess = "true", Tooltip = "캐릭터가 사용하는 무기 클래스"))
	TObjectPtr<UClass> WeaponClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ComboAttack, Meta = (AllowPrivateAccess = "true", Tooltip = "캐릭터가 사용하는 무기"))
	TObjectPtr<class AUPWeaponBase> Weapon;

public:
	FORCEINLINE AUPWeaponBase* GetWeapon() { return Weapon; }
// Attack Hit Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Init, Meta = (AllowPrivateAccess = "true", Tooltip = "피격시 생성되는 파티클"))
	TObjectPtr<UNiagaraSystem> HitEffect;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Init, Meta = (AllowPrivateAccess = "true", Tooltip = "피격시 생성되는 사운드"))
	TObjectPtr<USoundBase> HitSound;

	float CurAttackDamage;
	
	virtual float UPTakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Attack(FHitResult& InHit);

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
	
// Stat Section
public:
	FORCEINLINE const UUPCharacterStatComponent* GetStat() const { return StatComponent.Get(); }
	void ApplyStat(const FUPCharacterStat& BaseStat, const FUPCharacterStat& ModifierStat);

// Stun Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category =Init, Meta = (AllowPrivateAccess = "true", Tooltip = "캐릭터 스턴 애니메이션 몽타주"))
	TObjectPtr<class UAnimMontage> StunMontage;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=STAT)
	bool bIsStun;

	FORCEINLINE bool IsStun() const { return bIsStun; }
	virtual void SetStun();
	void PlayStunAnimation();
	UFUNCTION()
	virtual void StunAnimEnd(UAnimMontage* Montage, bool bInterrupted);
};
