// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Weapon/UPWeaponBase.h"

#include "Audio/UPAudioManager.h"
#include "Interface/UPDamageableInterface.h"
#include "Kismet/GameplayStatics.h"

class UUPAudioManager;

AUPWeaponBase::AUPWeaponBase()
{

}

void AUPWeaponBase::ClearAttackedActors()
{
	AttackedActors.Empty();
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
	UNiagaraSystem* DamageParticle = BaseEffect;
	if (Damageable->GetHitEffect() != nullptr)
	{
		DamageParticle = Damageable->GetHitEffect();
	}
	if (DamageParticle != nullptr)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DamageParticle, result.ImpactPoint, FRotator::ZeroRotator, FVector(1.f), true, true);
	}
	
	/* Sound */
	USoundBase* DamageSound = BaseHitSound;
	if (Damageable->GetHitSound() != nullptr)
	{
		DamageSound = Damageable->GetHitSound();
	}
	if (DamageSound != nullptr)
	{
		UUPAudioManager* AudioManager = GetGameInstance()->GetSubsystem<UUPAudioManager>();
		if (AudioManager != nullptr)
		{
			AudioManager->PlaySoundAtLocation(DamageSound, result.Location);
		}
	}
}
