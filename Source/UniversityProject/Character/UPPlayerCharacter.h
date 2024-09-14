// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/UPCharacterBase.h"
#include "InputActionValue.h"
#include "Components/TimelineComponent.h"
#include "Player/UPPlayerController.h"
#include "UPPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class UNIVERSITYPROJECT_API AUPPlayerCharacter : public AUPCharacterBase
{
	GENERATED_BODY()

public:
	AUPPlayerCharacter();

	virtual void PostInitializeComponents() override;

protected:
	virtual void BeginPlay() override;
	virtual void SetDead() override;
	virtual void Tick(float DeltaSeconds) override;

public:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	TObjectPtr<AUPPlayerController> PlayerController;
	TObjectPtr<UCharacterMovementComponent> CharacterMovementComponent;
	
// Camera Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UCameraComponent> FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Camera)
	TSubclassOf<UCameraShakeBase> HitCameraShake;

	void ShakeCamera(FHitResult& HitResult);

	UPROPERTY(EditAnywhere, Category="Camera")
	float MinZoom = 300.0f;

	UPROPERTY(EditAnywhere, Category="Camera")
	float MaxZoom = 800.0f;

	UPROPERTY(EditAnywhere, Category="Camera")
	float ZoomStep = 10.0f;

	UPROPERTY(EditAnywhere, Category="Camera")
	float CurrentZoom;

	UFUNCTION()
	void ZoomCamera(float Value);
// Animation Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Animation)
	float PauseDuration = 0.15f;

	void AnimationHitStop(FHitResult& HitResult);
	void SetHitStopTimer();
	void ResumeAnimation();
	
// Input Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> IMC_BackView;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AttackAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> DashAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> SprintAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> CameraZoomAction;
	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Attack(const FInputActionValue& Value);
	void Dash(const FInputActionValue& Value);
	void Sprint(const FInputActionValue& Value);
	void StopSprint(const FInputActionValue& Value);
	void ZoomCamera(const FInputActionValue& Value);
/* dash Section*/
protected:
	FOnTimelineFloat TimelineCallback;
	FOnTimelineEvent TimelineFinishedCallback;
	FVector DashStartLocation;
	FVector DashEndLocation;
	FVector DashEndVelocity;
	float DashDistance;
	FTimeline DashTimeline;

	
	void DashStart(FVector DashDirection, FVector DashVelocity);
	UFUNCTION()
	void UpdateDash(float Value);
	UFUNCTION()
	void FinishDash();
	
	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat* DashCurve;


};
