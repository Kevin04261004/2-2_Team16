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

void AUPPettuWeapon::CheckAttackRange(UUPPettuSkillData* SkillData)
{
	AActor* OwnerActor = GetOwner();
	FVector OwnerLocation = OwnerActor->GetActorLocation();
	FVector StartLocation = OwnerLocation + SkillData->GetCollisionLocation() +
		OwnerActor->GetActorForwardVector() * SkillData->GetAmount();
	FVector EndLocation = StartLocation + FVector(0.0f, 0.0f, 1.0f);
	AttackRadius = SkillData->GetSkillRange();
	CheckCollision(StartLocation, EndLocation);
}

void AUPPettuWeapon::CheckAttackSocket(FName SocketName, UUPPettuSkillData* SkillData,  USkeletalMeshComponent* MeshComp)
{
	AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereCollision->SetHiddenInGame(false);
	SphereCollision->SetSphereRadius(SkillData->GetSkillRange());
	SphereCollision->SetRelativeLocation(SkillData->GetCollisionLocation());
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
	TArray<FHitResult> HitResults;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(GetOwner());
	FVector StartLocation = Start; 
	FVector EndLocation = End;     
	ETraceTypeQuery TraceChannel = UEngineTypes::ConvertToTraceType(CCHANEL_UPACTION);
	FCollisionQueryParams CollisionParams(FName(TEXT("WeaponTrace")), false, GetOwner());
	
	bool bHit = UKismetSystemLibrary::SphereTraceMulti(
		GetWorld(),
		StartLocation,
		EndLocation,
		AttackRadius,
		TraceChannel,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration, 
		HitResults,
		true, 
		FLinearColor::Red,
		FLinearColor::Green,
		1.0f
	);

	
	for (FHitResult& Hit : HitResults)
	{
		if (Hit.GetActor() && Hit.GetActor()->IsA(AUPPlayerCharacter::StaticClass())) 
		{
			if (!AttackedActors.Contains(Hit.GetActor()))
			{
				Attack(Hit); 
				AttackedActors.Add(Hit.GetActor()); 
			}
		}
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
		FHitResult HitResult = SweepResult;
		Attack(HitResult);
	}
}
