// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UPHealthEntityCharacter.h"
#include "Components/UPCharacterMovementComponent.h"
#include "UPCharacterBase.generated.h"

UCLASS()
class UNIVERSITYPROJECT_API AUPCharacterBase : public AUPHealthEntityCharacter
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
	virtual void Attack(FHitResult& InHit);
	virtual void SetDead() override;
// Stun Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category =Init, Meta = (AllowPrivateAccess = "true", Tooltip = "캐릭터 스턴 애니메이션 몽타주"))
	TObjectPtr<class UAnimMontage> StunMontage;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=State)
	bool bIsStun;

	FORCEINLINE bool IsStun() const { return bIsStun; }
	virtual void SetStun();
	void PlayStunAnimation();
	UFUNCTION()
	virtual void StunAnimEnd(UAnimMontage* Montage, bool bInterrupted);

/* UI Section */
protected:
	virtual void SetupHUDWidget(class UUPHudWidget* InHUDWidget) {};

/* UI Section */
public:
	void SetUI();

public:
	UFUNCTION()
	void DestroyWeapon();
};
