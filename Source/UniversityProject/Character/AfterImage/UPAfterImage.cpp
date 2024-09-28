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
	ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_PoseMesh(TEXT("/Game/UniversityProject/UsingAssets/SK_CharM_Bladed.SK_CharM_Bladed"));
	if (SK_PoseMesh.Succeeded())
	{
		PoseableMesh->SetSkeletalMesh(SK_PoseMesh.Object);
	}
	ConstructorHelpers::FObjectFinder<UMaterialInstance> M_GhostTail(TEXT("/Game/UniversityProject/Graphics/Effects/afterImage/M_AfterImage_Inst.M_AfterImage_Inst"));
	if (M_GhostTail.Succeeded())
	{
		GhostMaterial = M_GhostTail.Object;
	}
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
	PoseableMesh->CopyPoseFromSkeletalComponent(Mesh);
	
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


