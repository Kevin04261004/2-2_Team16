// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Weapon/UPPettuWeapon.h"
#include "Physics/Collision.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

AUPPettuWeapon::AUPPettuWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AUPPettuWeapon::CheckAttackRange()
{
	CheckCollision();
}

void AUPPettuWeapon::AttackedActorsClear()
{
	AttackedActors.Empty();
}

void AUPPettuWeapon::CheckCollision()
{
	FHitResult HitResult;

	FVector StartLocation = GetOwner()->GetActorLocation();
	FVector EndLocation = StartLocation + GetOwner()->GetActorForwardVector() * AttackRange;

	FCollisionQueryParams CollisionParams = FCollisionQueryParams(FName(TEXT("WeaponTrace")), false, GetOwner());

	bool bHit = GetWorld()->SweepSingleByChannel(HitResult,
		StartLocation,
		EndLocation,
		FQuat::Identity,
		CCHANEL_UPACTION,
		FCollisionShape::MakeSphere(AttackRadius),
		CollisionParams);

	if (bHit && HitResult.GetActor() && !AttackedActors.Contains(HitResult.GetActor()))
	{
		Attack(HitResult);
	}

	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 2.0f);
}

void AUPPettuWeapon::AttackSuccess(FHitResult& result, IUPDamageableInterface* Damageable)
{
	Super::AttackSuccess(result, Damageable);

	// TODO: 공격 성공 시 추가로 처리할 내용을 작성
}
