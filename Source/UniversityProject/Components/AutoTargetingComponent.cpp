// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AutoTargetingComponent.h"
#include <limits>
#include "Interface/UPDamageableInterface.h"
#include "Physics/Collision.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UAutoTargetingComponent::UAutoTargetingComponent()
{
	TargetSearchRadius = 2000.0f;
	TurnSpeed = 2.0f;
}

AActor* UAutoTargetingComponent::FindDamageableTargetOrNull(const FVector& center, EAutoTargetingMode TargetingMode)
{
	TArray<FHitResult> HitResults;

	FCollisionShape Sphere = FCollisionShape::MakeSphere(TargetSearchRadius);
	FCollisionQueryParams Parameters;
	Parameters.AddIgnoredActor(GetOwner());
	
	DrawDebugSphere(GetWorld(), center, TargetSearchRadius, 12, FColor::Blue, false, 1.5f);
	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		center,
		center,
		FQuat::Identity,
		ECC_Visibility,
		Sphere,
		Parameters
	);

	// TArray<AActor*> ActorsToIgnore;
	// ActorsToIgnore.Add(GetOwner());
	// ETraceTypeQuery TraceChannel = UEngineTypes::ConvertToTraceType(CCHANEL_UPACTION);
	//
	// bool bHit = UKismetSystemLibrary::SphereTraceMulti(
	// 	GetWorld(),
	// 	center,
	// 	center,
	// 	5000,
	// 	TraceChannel,
	// 	false,
	// 	ActorsToIgnore,
	// 	EDrawDebugTrace::None,
	//   HitResults,
	//   true,
	//   FLinearColor::Red,
	//   FLinearColor::Green,
	//   1.0f);
	
	if (!bHit)
	{
		return nullptr;
	}

	TArray<AActor*> FoundTargets;

	for (const FHitResult& HitResult : HitResults)
	{
		if (HitResult.GetActor() == GetOwner())
		{
			continue;
		}
		IUPDamageableInterface* curTarget = Cast<IUPDamageableInterface>(HitResult.GetActor());
		if (curTarget == nullptr)
		{
			continue;
		}
		FoundTargets.Add(HitResult.GetActor());
	}
	
	switch (TargetingMode)
	{
	case EAutoTargetingMode::ATM_Nearest:
		return FindNearestTarget(center, FoundTargets);
	case EAutoTargetingMode::ATM_Farthest:
		return FindFarthestTarget(center, FoundTargets);
	case EAutoTargetingMode::ATM_Strongest:
		return FindStrongestTarget(center, FoundTargets);
	case EAutoTargetingMode::ATM_Weakest:
		return FindWeakestTarget(center, FoundTargets);
	default:
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, "TargetMode Not Founded!!!");
		return nullptr;
	}
	return nullptr;
}

void UAutoTargetingComponent::RotateToTarget(const FVector& targetLocation)
{
	AActor* owner = GetOwner();
	check(owner != nullptr);
	FVector playerLocation = owner->GetActorLocation();
	TargetLocation = targetLocation;

	if (TargetLocation == playerLocation)
	{
		return;
	}
	
	LookAtRotation = UKismetMathLibrary::FindLookAtRotation(playerLocation, TargetLocation);
	
	LookAtRotation.Pitch = 0.0f;
	LookAtRotation.Roll = 0.0f;
	
	GetOwner()->SetActorRotation(LookAtRotation);
}

#pragma optimize("", off)
void UAutoTargetingComponent::RotateToTarget(AActor* target)
{
	AActor* owner = GetOwner();
	check(owner != nullptr);
	FVector playerLocation = owner->GetActorLocation();
	TargetLocation = target->GetActorLocation();

	if (TargetLocation == playerLocation)
	{
		return;
	}
	
	LookAtRotation = UKismetMathLibrary::FindLookAtRotation(playerLocation, TargetLocation);
	
	LookAtRotation.Pitch = 0.0f;
	LookAtRotation.Roll = 0.0f;
	
	owner->SetActorRotation(LookAtRotation);
}
#pragma optimize("", on)

AActor* UAutoTargetingComponent::FindNearestTarget(const FVector& center, TArray<AActor*> targets)
{
	float minDistance = FLT_MAX;
	AActor* nearest = nullptr;
	
	for (AActor* curTarget : targets)
	{
		float curDistance = FVector::Distance(center, curTarget->GetActorLocation());
		if (curDistance < minDistance)
		{
			nearest = curTarget;
			minDistance = curDistance;
		}
	}
	return nearest;
}

AActor* UAutoTargetingComponent::FindFarthestTarget(const FVector& center, TArray<AActor*> targets)
{
	float maxDistance = FLT_MIN;
	AActor* farthest = nullptr;
	
	for (AActor* curTarget : targets)
	{
		float curDistance = FVector::Distance(center, curTarget->GetActorLocation());
		if (curDistance > maxDistance)
		{
			farthest = curTarget;
			maxDistance = curDistance;
		}
	}
	return farthest;
}

AActor* UAutoTargetingComponent::FindStrongestTarget(const FVector& center, TArray<AActor*> targets)
{
	// TODO: 전투력 시스템 넣으면 추가하면 좋을 듯? 근데 우리 게임에서는 굳이?
	return FindNearestTarget(center, targets);
}

AActor* UAutoTargetingComponent::FindWeakestTarget(const FVector& center, TArray<AActor*> targets)
{
	// TODO: 전투력 시스템 넣으면 추가하면 좋을 듯? 근데 우리 게임에서는 굳이?
	return FindNearestTarget(center, targets);
}
