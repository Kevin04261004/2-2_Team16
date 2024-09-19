// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/UPCameraComponent.h"

#include "Player/UPPlayerController.h"

UUPCameraComponent::UUPCameraComponent()
{
	
}

void UUPCameraComponent::Initialize(USpringArmComponent& SpringArmComponent, UCameraComponent& CameraComponent)
{
	Super::InitializeComponent();

	CameraBoom = SpringArmComponent;
	FollowCamera = CameraComponent;
	CurrentZoom = CameraBoom->TargetArmLength;
}


void UUPCameraComponent::ShakeCamera(FHitResult& HitResult)
{
	AActor* OwnerActor = GetOwner();
	if (OwnerActor == nullptr || HitCameraShake == nullptr)
	{
		return;
	}

	AUPPlayerController* PlayerController = Cast<AUPPlayerController>(OwnerActor->GetInstigatorController());
	if (PlayerController == nullptr || HitCameraShake == nullptr)
	{
		return;
	}
	PlayerController->ClientStartCameraShake(HitCameraShake);
}

void UUPCameraComponent::ZoomCamera(float Value)
{
	if (Value != 0.0f)
	{
		CurrentZoom = FMath::Clamp(CurrentZoom - (Value * ZoomStep), MinZoom, MaxZoom);
		CameraBoom->TargetArmLength = CurrentZoom;
	}
}