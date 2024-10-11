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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Init, Meta=(AllowPrivateAccess=true))
	TObjectPtr<UMaterialInstance> GhostMaterial;
	UMaterialInstanceDynamic* Material;

	bool IsSpawned = false;
	float FadeCountDown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Init, Meta=(AllowPrivateAccess=true))
	float FadeOutTime = 0.3f;
};
