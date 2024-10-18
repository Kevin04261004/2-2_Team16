// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/UPInputHandlerComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Character/UPCharacterBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Manager/UPTimeManager.h"
#include "Player/UPPlayerController.h"

class AUPPlayerController;

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
	EnhancedInputComponent->BindAction(GlobalTimeUp, ETriggerEvent::Triggered, this, &UUPInputHandlerComponent::GlobalTimeUpInputAction);
	EnhancedInputComponent->BindAction(GlobalTimeDown, ETriggerEvent::Triggered, this, &UUPInputHandlerComponent::GlobalTimeDownInputAction);
	EnhancedInputComponent->BindAction(GlobalTimeReset, ETriggerEvent::Triggered, this, &UUPInputHandlerComponent::GlobalTimeResetInputAction);
	EnhancedInputComponent->BindAction(GameExitAction, ETriggerEvent::Triggered, this, &UUPInputHandlerComponent::ExitGameInputAction);
	EnhancedInputComponent->BindAction(SettingAction, ETriggerEvent::Triggered, this, &UUPInputHandlerComponent::SettingInputAction);
}


void UUPInputHandlerComponent::MoveInputAction(const FInputActionValue& Value)
{
	MovementVector = Value.Get<FVector2D>();
}

void UUPInputHandlerComponent::LookInputAction(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	OnCameraLookInputed.Broadcast(LookAxisVector);
}

void UUPInputHandlerComponent::BaseAttackInputAction(const FInputActionValue& Value)
{
	OnBaseAttackInputed.Broadcast();
}

void UUPInputHandlerComponent::DashInputAction(const FInputActionValue& Value)
{
	OnDashInputed.Broadcast();
}

void UUPInputHandlerComponent::JumpInputAction(const FInputActionValue& Value)
{
	OnJumpInputed.Broadcast();
}

void UUPInputHandlerComponent::SprintInputAction(const FInputActionValue& Value)
{
	bIsSprint = Value.Get<bool>();
}

void UUPInputHandlerComponent::ZoomCameraInputAction(const FInputActionValue& Value)
{
	float zoomAxis = Value.Get<float>();
	OnCameraZoomed.Broadcast(zoomAxis);
}

void UUPInputHandlerComponent::GlobalTimeUpInputAction(const FInputActionValue& Value)
{
#if WITH_EDITOR || UE_BUILD_DEVELOPMENT
	if (UUPTimeManager* TimeManager = GetWorld()->GetGameInstance()->GetSubsystem<UUPTimeManager>())
	{
		TimeManager->WorldTimeUp();
	}
#endif
}

void UUPInputHandlerComponent::GlobalTimeDownInputAction(const FInputActionValue& Value)
{
#if WITH_EDITOR || UE_BUILD_DEVELOPMENT
	if (UUPTimeManager* TimeManager = GetWorld()->GetGameInstance()->GetSubsystem<UUPTimeManager>())
	{
		TimeManager->WorldTimeDown();
	}
#endif
}

void UUPInputHandlerComponent::GlobalTimeResetInputAction(const FInputActionValue& Value)
{
#if WITH_EDITOR || UE_BUILD_DEVELOPMENT
	if (UUPTimeManager* TimeManager = GetWorld()->GetGameInstance()->GetSubsystem<UUPTimeManager>())
	{
		TimeManager->WorldTimeReset();
	}
#endif
}

void UUPInputHandlerComponent::ExitGameInputAction(const FInputActionValue& Value)
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController == nullptr)
	{
		return;
	}
	UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, false);
}

void UUPInputHandlerComponent::SettingInputAction(const FInputActionValue& Value)
{
	AUPPlayerController* PlayerController = Cast<AUPPlayerController>(GetOwner()->GetInstigatorController());
	if (PlayerController == nullptr)
	{
		return;
	}
	if (PlayerController->GetInputMode() == EInputMode::Game)
	{
		PlayerController->SetUIMode();
	}
}

