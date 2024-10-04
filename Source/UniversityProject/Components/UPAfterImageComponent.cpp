// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/UPAfterImageComponent.h"

#include "Character/AfterImage/UPAfterImage.h"
#include "GameFramework/Character.h"

UUPAfterImageComponent::UUPAfterImageComponent()
{
	// Set Offset
	PositionOffset = FVector(0.f,0.f,-100.f);
	RotationOffset = FRotator(0.f,-100.f,0.f);
}

void UUPAfterImageComponent::Initialize()
{
	OwningCharacter = Cast<ACharacter>(GetOwner());
}

void UUPAfterImageComponent::CreateAfterImage() // IUPAfterImageableInterface
{
	check(AfterImageClass != nullptr);

	AActor* OwnerActor = GetOwner();
	
	FVector location = OwnerActor->GetActorLocation() + PositionOffset;
	FRotator rotation = OwnerActor->GetActorRotation() + RotationOffset;
	
	AUPAfterImage* afterImage = GetWorld()->SpawnActor<AUPAfterImage>(AfterImageClass, location, rotation);
	afterImage->Init(OwningCharacter->GetMesh());
}
