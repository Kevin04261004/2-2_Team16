// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Components/ActorComponent.h"
#include "UPInputHandlerComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCameraZoomed, float /* zoomValue */);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCameraLookInputed, FVector2D /* LookAxisVector */);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMoveInputed, FVector2D /* MoveAxisVector */);
DECLARE_MULTICAST_DELEGATE(FOnJumpInputed);
DECLARE_MULTICAST_DELEGATE(FOnDashInputed);
DECLARE_MULTICAST_DELEGATE(FOnUpperCutInputed);
DECLARE_MULTICAST_DELEGATE(FOnBaseAttackInputed);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNIVERSITYPROJECT_API UUPInputHandlerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UUPInputHandlerComponent();

	void SetMappingContext(const APlayerController* PlayerController) const;
	void BindActions(UEnhancedInputComponent* EnhancedInputComponent);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Init, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> IMC_BackView;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Init, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Init, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Init, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> DashAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Init, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Init, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> SprintAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Init, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> BaseAttackAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Init, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ESkillAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Init, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> QSkillAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Init, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> CameraZoomAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Init, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> SettingAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Init, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> TutoShowUIAction;
	/* Debug */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Init, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> GlobalTimeUp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Init, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> GlobalTimeDown;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Init, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> GlobalTimeReset;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Init, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> TutorialSkip;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Init, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> SkipOneStage;
	
	void MoveInputAction(const FInputActionValue& Value);
	void LookInputAction(const FInputActionValue& Value);
	void BaseAttackInputAction(const FInputActionValue& Value);
	void DashInputAction(const FInputActionValue& Value);
	void JumpInputAction(const FInputActionValue& Value);
	void SprintInputAction(const FInputActionValue& Value);
	void ZoomCameraInputAction(const FInputActionValue& Value);
	void GlobalTimeUpInputAction(const FInputActionValue& Value);
	void GlobalTimeDownInputAction(const FInputActionValue& Value);
	void GlobalTimeResetInputAction(const FInputActionValue& Value);
	void SettingInputAction(const FInputActionValue& Value);
	void TutorialSkipInputAction(const FInputActionValue& Value);
	void SkipOneStageInputAction(const FInputActionValue& Value);
	void TutoShowUIInputAction(const FInputActionValue& Value);
	
protected:
	FVector2D MovementVector;
	
public:
	FOnCameraZoomed OnCameraZoomed;
	FOnCameraLookInputed OnCameraLookInputed;
	FOnMoveInputed OnMoveInputed;
	FOnJumpInputed OnJumpInputed;
	FOnDashInputed OnDashInputed;
	FOnUpperCutInputed OnUpperCutInputed;
	FOnBaseAttackInputed OnBaseAttackInputed;
	
	FORCEINLINE FVector2D GetMovementVector() const { return MovementVector; }
	FORCEINLINE bool IsMoving() const { return MovementVector != FVector2D::ZeroVector; }
};
