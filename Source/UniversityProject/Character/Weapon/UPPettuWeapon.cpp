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
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SphereCollision->SetCollisionResponseToAllChannels(ECR_Overlap);
	SphereCollision->SetCollisionResponseToChannel(CCHANEL_UPACTION, ECR_Overlap);
	SphereCollision->SetHiddenInGame(true);
	
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AUPPettuWeapon::OnWeaponOverlapBegin);
	
	RootComponent = SphereCollision;
}

void AUPPettuWeapon::CheckAttackRange(float AttackRange, float Amount, FVector CollisionLocation)
{
	AActor* OwnerActor = GetOwner();
	if (OwnerActor == nullptr)
	{
		return;
	}
	FVector OwnerLocation = OwnerActor->GetActorLocation();
	FVector StartLocation = OwnerLocation + CollisionLocation +
		OwnerActor->GetActorForwardVector() * Amount;
	FVector EndLocation = StartLocation + FVector(0.0f, 0.0f, 1.0f);
	AttackRadius = AttackRange;
	CheckCollision(StartLocation, EndLocation);
}

void AUPPettuWeapon::CheckAttackSocket(FName SocketName, UUPPettuSkillData* SkillData,  USkeletalMeshComponent* MeshComp)
{
	AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereCollision->SetSphereRadius(SkillData->GetSkillRange());
	SphereCollision->SetRelativeLocation(SkillData->GetCollisionLocation());
}

void AUPPettuWeapon::CheckAttackRange(FName SocketName, USkeletalMeshComponent* MeshComp, float AttackRange, FVector CollisionLocation)
{
	AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereCollision->SetSphereRadius(AttackRange);
	SphereCollision->SetRelativeLocation(CollisionLocation);
}

void AUPPettuWeapon::SetCollision(bool bIsCollision)
{
	if (bIsCollision)
	{
		SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	else
	{
		SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
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
	
	// 넉백이 발생할 때 상대방에게 가할 힘의 크기
	float KnockbackStrength = 1000.0f;

	// 공격 성공 시 추가 처리 (넉백)
	if (AActor* HitActor = result.GetActor())
	{
		// 적 캐릭터가 물리 시뮬레이션이 가능하면 넉백 적용
		if (UPrimitiveComponent* HitComp = Cast<UPrimitiveComponent>(HitActor->GetRootComponent()))
		{
			if (HitComp->IsSimulatingPhysics())
			{
				// 공격자가 있는 방향을 기준으로 피격자를 밀어냄
				FVector KnockbackDirection = HitActor->GetActorLocation() - GetOwner()->GetActorLocation();
				KnockbackDirection.Z = 0.0f; // Z 축으로는 힘을 가하지 않음 (원하는 경우 Z 축도 포함 가능)
				KnockbackDirection.Normalize();

				// 충격(Impulse) 적용 (X, Y 축으로 밀어냄)
				HitComp->AddImpulse(KnockbackDirection * KnockbackStrength, NAME_None, true);
			}
		}
	}
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
