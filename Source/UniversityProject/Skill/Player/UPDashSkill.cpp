// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Player/UPDashSkill.h"

#include "UPSkillManagerComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Character/UPPlayerCharacter.h"
#include "Components/UPCharacterMovementComponent.h"

UUPDashSkill::UUPDashSkill()
{
}

void UUPDashSkill::BeginPlay()
{
	Super::BeginPlay();
	
	if (DashCurve)
	{
		TimelineCallback.BindUFunction(this, FName("UpdateDash"));
		DashTimeline.AddInterpFloat(DashCurve, TimelineCallback);
	
		TimelineFinishedCallback.BindUFunction(this, FName("FinishDash"));
		DashTimeline.SetTimelineFinishedFunc(TimelineFinishedCallback);
	}
}

void UUPDashSkill::CustomActivate_Implementation(AActor* TargetOrNull)
{
	Super::CustomActivate_Implementation(TargetOrNull);

	Dash();
}

void UUPDashSkill::CustomStop_Implementation()
{
	Super::CustomStop_Implementation();
}

void UUPDashSkill::CustomDeActivate_Implementation(AActor* TargetOrNull)
{
	Super::CustomDeActivate_Implementation(TargetOrNull);

	OnDashFinished.Broadcast();
}

void UUPDashSkill::Initialize(class UUPCharacterMovementComponent* InCharacterMovementComponent,
	class AUPPlayerCharacter* InPlayerCharacter)
{
	Super::Initialize(InCharacterMovementComponent, InPlayerCharacter);
	
}

void UUPDashSkill::Dash()
{
	FVector LastInputVector = CharacterMovementComponent->GetLastInputVector();
	if (!LastInputVector.IsNearlyZero())
	{
		FRotator NewRotation = LastInputVector.Rotation();
		GetOwner()->SetActorRotation(NewRotation);
	}
	
	FHitResult HitResult;

	bool bHit = PlayerCharacter->TryCheckForwardCollision(DashDistance, HitResult);
	
	if (bHit && FVector::Distance(GetOwner()->GetActorLocation(), HitResult.Location) < 150.0f)
    {
    	return;
    }
    	
    FVector DashLocation = bHit ? HitResult.Location - GetOwner()->GetActorForwardVector() * 150 : HitResult.TraceEnd;


	FVector DashVelocity = UAIBlueprintHelperLibrary::IsValidAIDirection(LastInputVector) ? LastInputVector : GetOwner()->GetActorForwardVector();

	DashStart(DashLocation, DashVelocity);
}

void UUPDashSkill::DashStart(FVector InDashEndLocation, FVector InDashVelocity)
{
	check(DashCurve != nullptr);

	DashStartLocation = GetOwner()->GetActorLocation();
	DashEndLocation = InDashEndLocation;
	DashEndVelocity = InDashVelocity;
	
	DashTimeline.SetPlayRate(1.0f / SkillData->GetSkillDuration(1.0f));
	DashTimeline.PlayFromStart();

	if (!GetOwner()->GetWorldTimerManager().IsTimerActive(DashTimerHandle))
	{
		GetOwner()->GetWorldTimerManager().SetTimer(DashTimerHandle, this, &UUPDashSkill::UpdateDashTimeline, 0.01f, true);
	}
}

void UUPDashSkill::UpdateDash(float Value)
{
	// 위치 보간
	FVector NewLocation = FMath::Lerp(DashStartLocation, DashEndLocation, Value);
	GetOwner()->SetActorLocation(NewLocation);
}

void UUPDashSkill::UpdateDashTimeline()
{
	if (DashTimeline.IsPlaying())
	{
		DashTimeline.TickTimeline(0.01f);

		float PlaybackPosition = DashTimeline.GetPlaybackPosition();
		float TimelineLength = DashTimeline.GetTimelineLength();
		float NormalizedPosition = PlaybackPosition / TimelineLength;
		
		UpdateDash(NormalizedPosition);
	}
	else
	{
		GetOwner()->GetWorldTimerManager().ClearTimer(DashTimerHandle);
	}
}

void UUPDashSkill::FinishDash()
{
	GetOwner()->GetWorldTimerManager().ClearTimer(DashTimerHandle);
	CharacterMovementComponent->Velocity = DashEndVelocity * 500.0f;
}