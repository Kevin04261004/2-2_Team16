// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Components/ActorComponent.h"
#include "UPInputHandlerComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCameraZoomed, float /* zoomValue */);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCameraLookInputed, FVector2D /* LookAxisVector */);

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
	
	void MoveInputAction(const FInputActionValue& Value);
	void LookInputAction(const FInputActionValue& Value);
	void BaseAttackInputAction(const FInputActionValue& Value);
	void DashInputAction(const FInputActionValue& Value);
	void JumpInputAction(const FInputActionValue& Value);
	void SprintInputAction(const FInputActionValue& Value);
	void ZoomCameraInputAction(const FInputActionValue& Value);

protected:
	FVector2D MovementVector;
	bool Jump;
	bool Sprint;
	
public:
	FOnCameraZoomed OnCameraZoomed;
	FOnCameraLookInputed OnCameraLookInputed;
	
	FORCEINLINE FVector2D GetMovementVector() { return MovementVector; }
	FORCEINLINE bool IsMoving() { return MovementVector.Size() > 0; }
	FORCEINLINE bool IsJump() { return Jump; }
	FORCEINLINE bool IsSprint() { return Sprint; }
};
