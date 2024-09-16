// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameData/UPCharacterStatData.h"
#include "GameData/UPCharacterStat.h"
#include "GameFramework/Character.h"
#include "Interface/UPAfterImageableInterface.h"
#include "Interface/UPAnimationAttackCheckInterface.h"
#include "Interface/UPCharacterGoForwardInterface.h"
#include "Interface/UPDamageableInterface.h"
#include "UPCharacterBase.generated.h"

UCLASS()
class UNIVERSITYPROJECT_API AUPCharacterBase : public ACharacter, public IUPDamageableInterface, public IUPAnimationAttackCheckInterface, public IUPCharacterGoForwardInterface, public IUPAfterImageableInterface
{
	GENERATED_BODY()

// Init Section
public:
	AUPCharacterBase();
	virtual void BeginPlay() override;
	
	virtual void PostInitializeComponents() override;

// Init Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Init, meta=(AllowPrivateAccess= "true", Tooltip = "캐릭터가 사용하는 애니메이션 클래스"))
	TObjectPtr<UClass> AnimInstanceClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category =Init, Meta = (AllowPrivateAccess = "true", Tooltip = "게임 시작 시 초기화 될 캐릭터의 스텟"))
	TObjectPtr<UUPCharacterStatData> CharacterInitalizeStatData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category =Init, Meta = (AllowPrivateAccess = "true", Tooltip = "캐릭터 죽음 애니메이션 몽타주"))
	TObjectPtr<class UAnimMontage> DeadMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category =Init, Meta = (AllowPrivateAccess = "true", Tooltip = "캐릭터 잔상 이펙트"))
	TObjectPtr<UClass> AfterImageClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category =Init, Meta = (AllowPrivateAccess = "true", Tooltip = "디버그 모드시, Lay가 보임."))
	bool IsDebugMode;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category =Init, Meta = (AllowPrivateAccess = "true", Tooltip = "캐릭터가 사용하는 무기 클래스"))
	TObjectPtr<UClass> WeaponClass;
// ComboAction Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ComboAttack, Meta = (AllowPrivateAccess = "true", Tooltip = "콤보 공격 컴포넌트"))
	TObjectPtr<class UUPComboAttackComponent> ComboAttack;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ComboAttack, Meta = (AllowPrivateAccess = "true", Tooltip = "캐릭터가 사용하는 무기"))
	TObjectPtr<class AUPWeapon> Weapon;
	
	virtual void AttackHitCheck() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= ComboAttack, Meta = (AllowPrivateAccess = "true", Tooltip = "공격 시 얼마나 앞으로 이동하는가"))
	float GoForwardDistance;
	
	UPrimitiveComponent* CollisionComponent;
	FTimerHandle PhysicsTimerHandle;
	virtual void GoForward() override;
	void SetPhysicsFalse();
	bool TryCheckForwardCollision(float InLineTraceDistance);

// Attack Hit Section
protected:
	virtual float UPTakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void Attack(FHitResult& InHit);

// Attack Hit Section
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Init, Meta = (AllowPrivateAccess = "true", Tooltip = "피격시 생성되는 파티클"))
	TObjectPtr<UParticleSystem> HitEffect;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Init, Meta = (AllowPrivateAccess = "true", Tooltip = "피격시 생성되는 사운드"))
	TObjectPtr<USoundBase> HitSound;
	FORCEINLINE virtual UParticleSystem* GetHitEffect() override { return HitEffect.Get(); }
	FORCEINLINE virtual USoundBase* GetHitSound() override { return HitSound.Get(); }
// Dead Section
protected:
	virtual void SetDead();
	void PlayDeadAnimation();

// Stat Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true", Tooltip = "스텟 컴포넌트"))
	TObjectPtr<class UUPCharacterStatComponent> StatComponent;

// Stat Section
public:
	FORCEINLINE const UUPCharacterStatComponent* GetStat() const { return StatComponent.Get(); }
	void ApplyStat(const FUPCharacterStat& BaseStat, const FUPCharacterStat& ModifierStat);

// After Image;
protected:
	virtual void CreateAfterImage() override;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AfterImage, Meta = (AllowPrivateAccess = "true", Tooltip = "캐릭터로부터 얼마나 떨어져서 생성되는지"))
	FVector PositionOffset;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AfterImage, Meta = (AllowPrivateAccess = "true", Tooltip = "캐릭터로부터 얼마나 회전해서 생성되는지"))
	FRotator RotationOffset;
};
