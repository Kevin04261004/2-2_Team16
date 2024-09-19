// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Weapon/UPWeaponBase.h"

#include "Interface/UPDamageableInterface.h"
#include "Kismet/GameplayStatics.h"

AUPWeaponBase::AUPWeaponBase()
{

}

void AUPWeaponBase::Attack(FHitResult& result)
{
	if (AttackedActors.Contains(result.GetActor()))
	{
		return;
	}
	AttackedActors.Add(result.GetActor());
	IUPDamageableInterface* Damageable = Cast<IUPDamageableInterface>(result.GetActor());
	if (Damageable == nullptr)
	{
		return;
	}

	AttackSuccess(result, Damageable);
	
	/* delegate */
	OnWeaponHit.Broadcast(result);
}

void AUPWeaponBase::AttackSuccess(FHitResult& result, IUPDamageableInterface* Damageable)
{
	/* Effect */
	UParticleSystem* DamageParticle = BaseHitEffect;
	if (Damageable->GetHitEffect() != nullptr)
	{
		DamageParticle = Damageable->GetHitEffect();
	}
	if (DamageParticle != nullptr)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DamageParticle, result.ImpactPoint);
	}
	
	/* Sound */
	USoundBase* DamageSound = BaseHitSound;
	if (Damageable->GetHitSound() != nullptr)
	{
		DamageSound = Damageable->GetHitSound();
	}
	if (DamageSound != nullptr)
	{
		UGameplayStatics::SpawnSound2D(GetWorld(), DamageSound);
	}
}
