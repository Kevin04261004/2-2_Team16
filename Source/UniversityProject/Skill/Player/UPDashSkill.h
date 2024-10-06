// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "Components/UPInputHandlerComponent.h"
#include "Skill/UPSkillBase.h"
#include "UPDashSkill.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDashFinished);

/**
 * 
 */
UCLASS()
class UNIVERSITYPROJECT_API UUPDashSkill : public UUPSkillBase
{
	GENERATED_BODY()

public:
	UUPDashSkill();

	FOnDashFinished OnDashFinished;

	virtual void Initialize(class UUPCharacterMovementComponent* InCharacterMovementComponent, class AUPPlayerCharacter* InPlayerCharacter) override;
	void Dash();

	virtual void BeginPlay() override;
protected:
	virtual void CustomActivate_Implementation(AActor* TargetOrNull) override;

	virtual void CustomStop_Implementation() override;

	virtual void CustomDeActivate_Implementation(AActor* TargetOrNull) override;

protected:
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
