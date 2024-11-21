// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AfterImage/UPAfterImage.h"

#include "Components/PoseableMeshComponent.h"
#include "Kismet/KismetMaterialLibrary.h"

// Sets default values
AUPAfterImage::AUPAfterImage()
{
	PrimaryActorTick.bCanEverTick = true;
	PoseableMesh = CreateDefaultSubobject<UPoseableMeshComponent>(TEXT("PoseableMesh"));
	RootComponent = PoseableMesh;
}

void AUPAfterImage::BeginPlay()
{
	Super::BeginPlay();
	PoseableMesh->SetVisibility(true, true);
}

void AUPAfterImage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsSpawned)
	{
		FadeCountDown -= DeltaTime;
		for (int32 i = 0; i < PoseableMesh->GetNumMaterials(); i++)
		{
			UMaterialInstanceDynamic* DynMaterial = Cast<UMaterialInstanceDynamic>(PoseableMesh->GetMaterial(i));
			if (DynMaterial)
			{
				DynMaterial->SetScalarParameterValue("Opacity", FadeCountDown / FadeOutTime);
			}
		}
		if (FadeCountDown <= 0.0f)
		{
			Destroy();
		}
	}
}

void AUPAfterImage::Init(USkeletalMeshComponent* Mesh)
{
	check (Mesh != nullptr);
	check (PoseableMesh != nullptr);
	check (Mesh->SkeletalMesh != nullptr);
	PoseableMesh->SetSkeletalMesh(Mesh->SkeletalMesh);
	PoseableMesh->CopyPoseFromSkeletalComponent(Mesh);
	PoseableMesh->OverlayMaterialMaxDrawDistance = 0.1f;

	Material = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), GhostMaterial);
	if (Material == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create dynamic material instance!"));
		return;
	}

	for (int32 i = 0; i < PoseableMesh->GetNumMaterials(); i++)
	{
		PoseableMesh->SetMaterial(i, Material);
	}

	FadeCountDown = FadeOutTime;
	IsSpawned = true;
}


