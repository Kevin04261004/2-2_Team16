// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraFunctionLibrary.h"
#include "UPWeaponBase.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnWeaponHit, FHitResult& /* Result */);

UCLASS()
class UNIVERSITYPROJECT_API AUPWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUPWeaponBase();
	
	virtual void ClearAttackedActors();

	// delegate
	FOnWeaponHit OnWeaponHit;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	USkeletalMeshComponent* WeaponMesh;

	/* 상대방에 Effect가 없으면 실행되는 기본 이펙트 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effects")
	TObjectPtr<UNiagaraSystem> BaseEffect;

	/* 상대방에 Effect가 없으면 실행되는 기본 이펙트 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sounds")
	USoundBase* BaseHitSound;
	
	TArray<AActor*> AttackedActors;

	// 상대와 충돌함.
	void Attack(FHitResult& result);

	// 상속받은 무기에서 공격에 성공했을 때 무기 클래스에서 필요한 작업들. 만약 구현된 함수가 필요없으면 Super::AttackSuccess()를 제외할 것.
	virtual void AttackSuccess(FHitResult& result, class IUPDamageableInterface* Damageable);
};
