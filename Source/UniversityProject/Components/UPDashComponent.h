// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UPCharacterMovementComponent.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "UPDashComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNIVERSITYPROJECT_API UUPDashComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UUPDashComponent();

	void Initialize(class UUPCharacterMovementComponent* InCharacterMovementComponent, class AUPPlayerCharacter* InPlayerCharacter);
	void Dash();
protected:
	
	virtual void BeginPlay() override;
	
	UUPCharacterMovementComponent* CharacterMovementComponent;
	AUPPlayerCharacter* PlayerCharacter;
	
	FOnTimelineFloat TimelineCallback;
	FOnTimelineEvent TimelineFinishedCallback;
	FVector DashStartLocation;
	FVector DashEndLocation;
	FVector DashEndVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Init, Meta = (AllowPrivateAccess = true))
	float DashDistance;
	FTimeline DashTimeline;
	FTimerHandle DashTimerHandle;
	
	void DashStart(FVector InDashEndLocation, FVector InDashVelocity);
	void UpdateDash(float Value);
	void UpdateDashTimeline();
	void FinishDash();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Init, Meta = (AllowPrivateAccess = true))
	UCurveFloat* DashCurve;
};
