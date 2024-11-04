// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UPReachLocationCollision.generated.h"

UCLASS()
class UNIVERSITYPROJECT_API AUPReachLocationCollision : public AActor
{
	GENERATED_BODY()

public:
	AUPReachLocationCollision();

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> CylinderMesh;
	
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32
	                    OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
