// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UPAfterImage.generated.h"

UCLASS()
class UNIVERSITYPROJECT_API AUPAfterImage : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUPAfterImage();
	void Init(USkeletalMeshComponent* Mesh);
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	class UPoseableMeshComponent* PoseableMesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Material, Meta = (AllowPrivateAccess = "true"))
	UMaterialInstance* GhostMaterial;
	UMaterialInstanceDynamic* Material;

	float InitOpacity = 0.f;
	
	bool IsSpawned = false;
	float FadeCountDown;
	float FadeOutTime = 0.2f;
};
