// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/UPLerpToForwardAnimNotifyState.h"

#include "Character/UPCharacterBase.h"

void UUPLerpToForwardAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                  float InTotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, InTotalDuration);

	if (AActor* Owner = MeshComp->GetOwner())
	{
		StartLocation = Owner->GetActorLocation();

		AUPCharacterBase* base = Cast<AUPCharacterBase>(Owner);
		TargetLocation = StartLocation + Owner->GetActorForwardVector() * Amount;
		if (base != nullptr)
		{
			FHitResult Hit;
			FVector HitLocation;
			if (base->TryCheckForwardCollision(200, Hit, HitLocation))
			{
				TargetLocation = StartLocation;
			}
		}
		StartTime = MeshComp->GetWorld()->GetTimeSeconds();
		TotalDuration = InTotalDuration;
	}
	
}

void UUPLerpToForwardAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
	
	if (AActor* Owner = MeshComp->GetOwner())
	{
		// 현재 Notify 구간의 진행 시간 계산
		float CurrentTime = MeshComp->GetWorld()->GetTimeSeconds();
		float ElapsedTime = CurrentTime - StartTime; // Notify가 시작된 후 경과한 시간
		float Progress = FMath::Clamp(ElapsedTime / TotalDuration, 0.f, 1.f); // 0에서 1까지의 진행 비율 계산

		// 커브가 설정되어 있으면 커브의 값을 사용
		if (LerpCurve)
		{
			Progress = LerpCurve->GetFloatValue(Progress);
		}

		// 진행 비율에 따라 캐릭터의 위치를 업데이트
		FVector NewLocation = FMath::Lerp(StartLocation, TargetLocation, Progress);
		Owner->SetActorLocation(NewLocation);
	}
}

void UUPLerpToForwardAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	
	if (AActor* Owner = MeshComp->GetOwner())
	{
		// 캐릭터를 원래 위치로 다시 이동시킴
		Owner->SetActorLocation(TargetLocation);
	}
}
