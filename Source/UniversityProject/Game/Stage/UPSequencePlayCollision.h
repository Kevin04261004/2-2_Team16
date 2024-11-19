// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelSequence.h"
#include "GameFramework/Actor.h"
#include "UPSequencePlayCollision.generated.h"

UCLASS()
class UNIVERSITYPROJECT_API AUPSequencePlayCollision : public AActor
{
	GENERATED_BODY()
	
public:	
	AUPSequencePlayCollision();

	UPROPERTY(EditAnywhere)
	ULevelSequence* Sequence;

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> CollisionMesh;


	
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32
						OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
