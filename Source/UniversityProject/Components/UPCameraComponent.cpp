// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/UPCameraComponent.h"

#include <ThirdParty/ShaderConductor/ShaderConductor/External/SPIRV-Headers/include/spirv/unified1/spirv.h>

#include "Character/UPPlayerCharacter.h"
#include "Player/UPPlayerController.h"

UUPCameraComponent::UUPCameraComponent()
{
	OwningCharacter = Cast<AUPPlayerCharacter>(GetOwner());
}

void UUPCameraComponent::Initialize(USpringArmComponent& SpringArmComponent, UCameraComponent& CameraComponent)
{
	Super::InitializeComponent();

	CameraBoom = SpringArmComponent;
	FollowCamera = CameraComponent;
	CurrentZoom = CameraBoom->TargetArmLength;
}


void UUPCameraComponent::HitShakeCamera(FHitResult& HitResult)
{
	PlayerController = Cast<AUPPlayerController>(GetOwner()->GetInstigatorController());
	check(HitCameraShake != nullptr);
	PlayerController->ClientStartCameraShake(HitCameraShake);
}

void UUPCameraComponent::DamagedShakeCamera()
{
	PlayerController = Cast<AUPPlayerController>(GetOwner()->GetInstigatorController());

	check(DamagedCameraShake != nullptr);
	PlayerController->ClientStartCameraShake(DamagedCameraShake);
}

void UUPCameraComponent::DashShakeCamera()
{
	PlayerController = Cast<AUPPlayerController>(GetOwner()->GetInstigatorController());

	check(DashCameraShake != nullptr);
	PlayerController->ClientStartCameraShake(DashCameraShake);
}

void UUPCameraComponent::ZoomCamera(float Value)
{
	if (Value != 0.0f)
	{
		CurrentZoom = FMath::Clamp(CurrentZoom - (Value * ZoomStep), MinZoom, MaxZoom);
		CameraBoom->TargetArmLength = CurrentZoom;
	}
}

void UUPCameraComponent::LookCamera(FVector2D LookAxisVector)
{
	OwningCharacter->AddControllerYawInput(LookAxisVector.X);
	OwningCharacter->AddControllerPitchInput(LookAxisVector.Y);
}