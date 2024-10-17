// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Weapon/UPPettuWeapon.h"
#include "Physics/Collision.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Character/UPPlayerCharacter.h"
#include "Components/SphereComponent.h"

AUPPettuWeapon::AUPPettuWeapon()
{
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("AttackCollision"));
	SphereCollision->InitSphereRadius(50.0f);
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SphereCollision->SetCollisionResponseToAllChannels(ECR_Overlap);
	SphereCollision->SetCollisionResponseToChannel(CCHANEL_UPACTION, ECR_Overlap);
	
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AUPPettuWeapon::OnWeaponOverlapBegin);
	
	RootComponent = SphereCollision;
}

void AUPPettuWeapon::CheckAttackRange(EPettuSkillType SkillType)
{
	FVector StartLocation = GetOwner()->GetActorLocation();
	FVector EndLocation = StartLocation + GetOwner()->GetActorForwardVector();
	switch (SkillType)
	{
	case EPettuSkillType::TakeTurnGroundAttack:
		AttackRange = 1.0f;
		AttackRadius = TakeTurnGorundRange;
		break;
	case EPettuSkillType::JumpGroundAttack:
		AttackRange = 1.0f;
		AttackRadius = JumpGroundRange;
		break;
	case EPettuSkillType::TwoHandGroundAttack:
		AttackRange = 1.0f;
		AttackRadius = TwoHandGroundRange;
		break;
	}
	CheckCollision(StartLocation, EndLocation);
}

void AUPPettuWeapon::CheckAttackSocket(FName SocketName, EPettuSkillType SkillType,  USkeletalMeshComponent* MeshComp)
{
	AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereCollision->SetHiddenInGame(false);
	switch (SkillType)
	{
	case EPettuSkillType::SmashAttack:
		SphereCollision->SetSphereRadius(Smash1Range);
		break;
	case EPettuSkillType::SmashAttack2:
		SphereCollision->SetSphereRadius(Smash2Range);
		break;
	}
}

void AUPPettuWeapon::SetCollision()
{
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SphereCollision->SetHiddenInGame(true);
}

void AUPPettuWeapon::ClearAttackedActors()
{
	Super::ClearAttackedActors();
}

void AUPPettuWeapon::CheckCollision(FVector Start, FVector End)
{
	ClearAttackedActors();
	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(GetOwner());
	FVector StartLocation = Start;
	FVector EndLocation = End;
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
	}
}

void AUPPettuWeapon::AttackSuccess(FHitResult& result, IUPDamageableInterface* Damageable)
{
	Super::AttackSuccess(result, Damageable);

	// TODO: 공격 성공 시 추가로 처리할 내용을 작성
}

void AUPPettuWeapon::OnWeaponOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && OtherActor != GetOwner())
	{
		ClearAttackedActors();
		GEngine->AddOnScreenDebugMessage( -1, 1.0f, FColor::Red, TEXT("OnWeaponOverlapBegin") );
		FHitResult HitResult = SweepResult;
		Attack(HitResult);
	}
}
