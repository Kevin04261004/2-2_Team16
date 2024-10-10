// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Weapon/UPPettuWeapon.h"
#include "Physics/Collision.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Character/UPPlayerCharacter.h"

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
	AttackedActorsClear();
	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(GetOwner());
	FVector StartLocation = GetOwner()->GetActorLocation();
	FVector EndLocation = StartLocation + GetOwner()->GetActorForwardVector() * AttackRange;
	ETraceTypeQuery TraceChannel = UEngineTypes::ConvertToTraceType(CCHANEL_UPACTION);
	FCollisionQueryParams CollisionParams = FCollisionQueryParams(FName(TEXT("WeaponTrace")), false, GetOwner());

	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("CheckCollision"));
	
	bool bHit = UKismetSystemLibrary::SphereTraceSingle(GetWorld(),
		StartLocation,
		EndLocation,
		AttackRadius,
		TraceChannel,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		HitResult,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		1.0f);

	if (bHit && HitResult.GetActor() && !AttackedActors.Contains(HitResult.GetActor()))
	{
		Attack(HitResult);
		AUPPlayerCharacter* PlayerCharacter = Cast<AUPPlayerCharacter>(HitResult.GetActor());
	}

	//DrawDebugSphere(GetWorld(), StartLocation, AttackRadius, 10.0f, FColor::Red, false, 2.0f);
}

void AUPPettuWeapon::AttackSuccess(FHitResult& result, IUPDamageableInterface* Damageable)
{
	Super::AttackSuccess(result, Damageable);

	// TODO: 공격 성공 시 추가로 처리할 내용을 작성
}
