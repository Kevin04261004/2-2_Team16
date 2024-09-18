// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/UPCharacterBase.h"
#include "InputActionValue.h"
#include "Components/TimelineComponent.h"
#include "Interface/UPAfterImageableInterface.h"
#include "Interface/UPAnimationAttackCheckInterface.h"
#include "Interface/UPCharacterGoForwardInterface.h"
#include "Player/UPPlayerController.h"
#include "UPPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class UNIVERSITYPROJECT_API AUPPlayerCharacter : public AUPCharacterBase, public IUPAnimationAttackCheckInterface, public IUPCharacterGoForwardInterface, public IUPAfterImageableInterface
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
	
// Camera Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UCameraComponent> FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Camera, Meta = (AllowPrivateAccess = true))
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
// Input Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Init, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> IMC_BackView;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Init, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Init, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AttackAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Init, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> DashAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Init, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> SprintAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Init, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Init, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> CameraZoomAction;
	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Attack(const FInputActionValue& Value);
	void Dash(const FInputActionValue& Value);
	void Sprint(const FInputActionValue& Value);
	void Walk(const FInputActionValue& Value);
	void ZoomCamera(const FInputActionValue& Value);
/* dash Section */
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
// After Image;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category =Init, Meta = (AllowPrivateAccess = "true", Tooltip = "캐릭터 잔상 이펙트"))
	TObjectPtr<UClass> AfterImageClass;
	
	virtual void CreateAfterImage() override;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AfterImage, Meta = (AllowPrivateAccess = "true", Tooltip = "캐릭터로부터 얼마나 떨어져서 생성되는지"))
	FVector PositionOffset;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AfterImage, Meta = (AllowPrivateAccess = "true", Tooltip = "캐릭터로부터 얼마나 회전해서 생성되는지"))
	FRotator RotationOffset;
// ComboAction Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ComboAttack, Meta = (AllowPrivateAccess = "true", Tooltip = "콤보 공격 컴포넌트"))
	TObjectPtr<class UUPComboAttackComponent> ComboAttack;
	
	virtual void AttackHitCheck() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= Init, Meta = (AllowPrivateAccess = "true", Tooltip = "공격 시 얼마나 앞으로 이동하는가"))
	float GoForwardDistance;
	
	UPrimitiveComponent* CollisionComponent;
	FTimerHandle PhysicsTimerHandle;
	virtual void GoForward() override;
	void SetPhysicsFalse();
};
