// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/ActorComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "UPCameraComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNIVERSITYPROJECT_API UUPCameraComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UUPCameraComponent();

	void Initialize(USpringArmComponent&, UCameraComponent&);
	
	void ShakeCamera(FHitResult& HitResult);

	void ZoomCamera(float Value);
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UCameraComponent> FollowCamera;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Camera, Meta = (AllowPrivateAccess = true))
	TSubclassOf<UCameraShakeBase> HitCameraShake;

	UPROPERTY(EditAnywhere, Category="Camera")
	float MinZoom = 300.0f;

	UPROPERTY(EditAnywhere, Category="Camera")
	float MaxZoom = 800.0f;

	UPROPERTY(EditAnywhere, Category="Camera")
	float ZoomStep = 10.0f;

	UPROPERTY(EditAnywhere, Category="Camera")
	float CurrentZoom;
};
