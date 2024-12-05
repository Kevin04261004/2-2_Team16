// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/UPLerpToForwardAnimNotifyState.h"

#include "Character/UPCharacterBase.h"
#include "Character/UPPlayerCharacter.h"
#include "Components/AutoTargetingComponent.h"
#include "Components/CapsuleComponent.h"
#include "Physics/Collision.h"
#include "Skill/Player/UPSkillManagerComponent.h"

struct FNotifyStateData
{
	FVector StartLocation;
	FVector TargetLocation;
	float TotalDuration;
	float StartTime;
};

TMap<AActor*, FNotifyStateData> NotifyStateDataMap;

void UUPLerpToForwardAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                  float InTotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, InTotalDuration);

	if (AActor* Owner = MeshComp->GetOwner())
	{
		if (bIsLookLastInput)
		{
			FVector LastInputVector = FVector::ZeroVector;
			if (AUPPlayerCharacter* PlayerCharacter = Cast<AUPPlayerCharacter>(Owner))
			{
				LastInputVector = PlayerCharacter->MovementComponent->GetLastInputVector();
			}
			if (!LastInputVector.IsNearlyZero())
			{
				FRotator NewRotation = LastInputVector.Rotation();
				Owner->SetActorRotation(NewRotation);
			}	
		}

		if (bLookTarget)
		{
			if (AUPPlayerCharacter* PlayerCharacter = Cast<AUPPlayerCharacter>(Owner))
			{
				UAutoTargetingComponent* AutoTargetingComponent = PlayerCharacter->GetSkillManager()->GetAutoTargetingComponent();
				AActor* target = AutoTargetingComponent->FindDamageableTargetOrNull(PlayerCharacter->GetActorLocation(), EAutoTargetingMode::ATM_Nearest);
				if (target != nullptr)
				{
					AutoTargetingComponent->RotateToTarget(target->GetActorLocation());
				}
			}
		}
		
		FVector StartLocation = Owner->GetActorLocation();

		AUPCharacterBase* base = Cast<AUPCharacterBase>(Owner);
		FVector TargetLocation = StartLocation + Owner->GetActorForwardVector() * Amount;
		if (base != nullptr)
		{
			FHitResult Hit;
			FVector HitLocation;

			// TODO: 무기의 길이를 구하는 코드 만들기.
			if (base->TryCheckForwardCollision(Amount >= 0 ? Amount : -Amount, Hit, HitLocation))
			{
				if (!bIgnoreEnemys || !IsTargetIsEnemy(MeshComp->GetOwner(), Hit.GetActor()))
				{
					Hit.Location.Z = Owner->GetActorLocation().Z;
					TargetLocation = Hit.Location - Owner->GetActorForwardVector() * (base->GetCapsuleComponent()->GetScaledCapsuleRadius() / 2);
				}
			}
		}

		FNotifyStateData NotifyData;
		NotifyData.StartLocation = StartLocation;
		NotifyData.TargetLocation = TargetLocation;
		NotifyData.StartTime = MeshComp->GetWorld()->GetTimeSeconds();
		NotifyData.TotalDuration = InTotalDuration;
		NotifyStateDataMap.Add(Owner, NotifyData);
		//StartTime = MeshComp->GetWorld()->GetTimeSeconds();
		//TotalDuration = InTotalDuration;
	}
	
}

void UUPLerpToForwardAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
	
	if (AActor* Owner = MeshComp->GetOwner())
	{
		if (FNotifyStateData* NotifyData = NotifyStateDataMap.Find(Owner))
		{
			// 현재 Notify 구간의 진행 시간 계산
			float CurrentTime = MeshComp->GetWorld()->GetTimeSeconds();
			float ElapsedTime = CurrentTime - NotifyData->StartTime; // Notify가 시작된 후 경과한 시간
			float Progress = FMath::Clamp(ElapsedTime / NotifyData->TotalDuration, 0.f, 1.f); // 0에서 1까지의 진행 비율 계산

			// 커브가 설정되어 있으면 커브의 값을 사용
			if (LerpCurve)
			{
				Progress = LerpCurve->GetFloatValue(Progress);
			}

			// 진행 비율에 따라 캐릭터의 위치를 업데이트
			FVector NewLocation = FMath::Lerp(NotifyData->StartLocation, NotifyData->TargetLocation, Progress);
			Owner->SetActorLocation(NewLocation);
		}
	}
}

void UUPLerpToForwardAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	
	if (AActor* Owner = MeshComp->GetOwner())
	{
		if (FNotifyStateData* NotifyData = NotifyStateDataMap.Find(Owner))
		{
			// 캐릭터를 원래 위치로 다시 이동시킴
			Owner->SetActorLocation(NotifyData->TargetLocation);
			NotifyStateDataMap.Remove(Owner);
		}
	}
}

bool UUPLerpToForwardAnimNotifyState::IsTargetIsEnemy(AActor* Owner, AActor* Target)
{
	TArray<FHitResult> HitResults;

	FCollisionShape Sphere = FCollisionShape::MakeSphere(Amount);
	FCollisionQueryParams Parameters;
	Parameters.AddIgnoredActor(Owner);
	
	DrawDebugSphere(GetWorld(), Owner->GetActorLocation(), Amount, 12, FColor::Blue, false, 1.5f);
	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		Owner->GetActorLocation(),
		Owner->GetActorLocation(),
		FQuat::Identity,
		CCHANEL_UPACTION,
		Sphere,
		Parameters
	);

	if (!bHit)
	{
		return false;
	}

	for (const FHitResult& HitResult : HitResults)
	{
		if (Target == HitResult.GetActor())
		{
			return true;
		}
	}
	return false;
}
