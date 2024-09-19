// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsControlComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNIVERSITYPROJECT_API UPhysicsControlComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPhysicsControlComponent();

	void Initialize();
	void GoForward(float distance);
protected:
	UPrimitiveComponent* CollisionComponent;
	FTimerHandle PhysicsTimerHandle;

	void SetPhysicsFalse();
};
