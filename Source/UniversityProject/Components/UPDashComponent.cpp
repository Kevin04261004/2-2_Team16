// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/UPDashComponent.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Character/UPPlayerCharacter.h"

UUPDashComponent::UUPDashComponent()
{
	
}

void UUPDashComponent::BeginPlay()
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

void UUPDashComponent::Initialize(UUPCharacterMovementComponent* InCharacterMovementComponent, AUPPlayerCharacter* InPlayerCharacter)
{
	CharacterMovementComponent = InCharacterMovementComponent;
	PlayerCharacter = InPlayerCharacter;
}

void UUPDashComponent::Dash()
{
	FVector LastInputVector = CharacterMovementComponent->GetLastInputVector();
	
	FHitResult HitResult;
	FVector ActorLocation;
	bool bHit = PlayerCharacter->TryCheckForwardCollision(DashDistance, HitResult, ActorLocation);
	HitResult.Location.Z = ActorLocation.Z;
	HitResult.TraceEnd.Z = ActorLocation.Z;
	if (bHit && FVector::Distance(GetOwner()->GetActorLocation(), HitResult.Location) < 150.0f)
    {
    	return;
    }
    	
    FVector DashLocation = bHit ? HitResult.Location - GetOwner()->GetActorForwardVector() * 150 : HitResult.TraceEnd;


	FVector DashVelocity = UAIBlueprintHelperLibrary::IsValidAIDirection(LastInputVector) ? LastInputVector : GetOwner()->GetActorForwardVector();

	DashStart(DashLocation, DashVelocity);
}

void UUPDashComponent::DashStart(FVector InDashEndLocation, FVector InDashVelocity)
{
	check(DashCurve != nullptr);

	DashStartLocation = GetOwner()->GetActorLocation();
	DashEndLocation = InDashEndLocation;
	DashEndVelocity = InDashVelocity;

	DashTimeline.PlayFromStart();

	if (!GetOwner()->GetWorldTimerManager().IsTimerActive(DashTimerHandle))
	{
		GetOwner()->GetWorldTimerManager().SetTimer(DashTimerHandle, this, &UUPDashComponent::UpdateDashTimeline, 0.01f, true);
	}
}

void UUPDashComponent::UpdateDash(float Value)
{
	// 위치 보간
	FVector NewLocation = FMath::Lerp(DashStartLocation, DashEndLocation, Value);
	GetOwner()->SetActorLocation(NewLocation);
}

void UUPDashComponent::UpdateDashTimeline()
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

void UUPDashComponent::FinishDash()
{
	GetOwner()->GetWorldTimerManager().ClearTimer(DashTimerHandle);
	CharacterMovementComponent->Velocity = DashEndVelocity * 500.0f;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, TEXT("Dash End"));
	}
}
