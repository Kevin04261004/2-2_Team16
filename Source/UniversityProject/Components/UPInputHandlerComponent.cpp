// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/UPInputHandlerComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

UUPInputHandlerComponent::UUPInputHandlerComponent()
{

}

void UUPInputHandlerComponent::SetMappingContext(const APlayerController* PlayerController) const
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(IMC_BackView, 0);
	}
}

void UUPInputHandlerComponent::BindActions(UEnhancedInputComponent* EnhancedInputComponent)
{
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &UUPInputHandlerComponent::MoveInputAction);
	EnhancedInputComponent->BindAction(BaseAttackAction, ETriggerEvent::Triggered, this, &UUPInputHandlerComponent::BaseAttackInputAction);
	EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &UUPInputHandlerComponent::DashInputAction);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &UUPInputHandlerComponent::JumpInputAction);
	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &UUPInputHandlerComponent::SprintInputAction);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &UUPInputHandlerComponent::LookInputAction);
	EnhancedInputComponent->BindAction(CameraZoomAction, ETriggerEvent::Triggered, this, &UUPInputHandlerComponent::ZoomCameraInputAction);
}


void UUPInputHandlerComponent::MoveInputAction(const FInputActionValue& Value)
{
	MovementVector = Value.Get<FVector2D>();
	
	// const FRotator Rotation = Controller->GetControlRotation();
	// const FRotator YawRotation(0, Rotation.Yaw, 0);
	//
	// const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	// const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	//
	// AddMovementInput(ForwardDirection, MovementVector.X);
	// AddMovementInput(RightDirection, MovementVector.Y);
}

void UUPInputHandlerComponent::LookInputAction(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	OnCameraLookInputed.Broadcast(LookAxisVector);
}

void UUPInputHandlerComponent::BaseAttackInputAction(const FInputActionValue& Value)
{
	
}

void UUPInputHandlerComponent::DashInputAction(const FInputActionValue& Value)
{
	// DashComponent->Dash();
}

void UUPInputHandlerComponent::JumpInputAction(const FInputActionValue& Value)
{
	Jump = Value.Get<bool>();
	// GetCharacterMovement()->DoJump(Jump);
}

void UUPInputHandlerComponent::SprintInputAction(const FInputActionValue& Value)
{
	Sprint = Value.Get<bool>();
	// MovementComponent->SetIsSprinting(Sprint);
}

void UUPInputHandlerComponent::ZoomCameraInputAction(const FInputActionValue& Value)
{
	float zoomAxis = Value.Get<float>();
	OnCameraZoomed.Broadcast(zoomAxis);
	// CameraComponent->ZoomCamera(zoomAxis);
}

