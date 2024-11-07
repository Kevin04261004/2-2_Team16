// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/UPPCAnimInstance.h"
#include "Character/UPPlayerCharacter.h"
#include "Character/Enemy/UPPettuCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

UUPPCAnimInstance::UUPPCAnimInstance()
{
	PettuCharacter = nullptr;
}

void UUPPCAnimInstance::UpdateLookAt(float DeltaTime, AUPPlayerCharacter* PlayerCharacter, AUPPettuCharacter* BossCharacter)
{
	if (!PlayerCharacter || !BossCharacter)
	{
		return; 
	}

	FVector ForwardVector = PlayerCharacter->GetActorForwardVector();
	FVector HeadSocketLocation = PlayerCharacter->GetMesh()->GetSocketLocation(TEXT("Bip001-Head"));
	FVector PelvisSocketLocation = BossCharacter->GetMesh()->GetSocketLocation(TEXT("Bip001-Pelvis"));

	FVector LookAtTarget;
	
	FRotator PlayerRotation = PlayerCharacter->GetActorRotation();
	FVector PlayerLocation = PlayerCharacter->GetActorLocation();
	FVector BossLocation = BossCharacter->GetActorLocation();
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(BossLocation, PlayerLocation);

	float ClampLookAtRotationYaw = UKismetMathLibrary::ClampAngle(LookAtRotation.Yaw, -55.0f, 55.0f);
	float YawDifference = FMath::Abs(PlayerRotation.Yaw - ClampLookAtRotationYaw);

	if (YawDifference > 55.0f)
	{
		FVector Forward = FVector(ForwardVector.X, ForwardVector.Y, HeadSocketLocation.Z);
		LookAtTarget = Forward;
		FVector CurrentLookAtPos = GetLookAtPos(); 
		FVector InterpolatedLookAtPos = FMath::VInterpTo(CurrentLookAtPos, LookAtTarget, DeltaTime, 3.0f); 
		SetLookAtPos(InterpolatedLookAtPos);
	}
	else
	{
		LookAtTarget = PelvisSocketLocation;
		SetLookAtPos(LookAtTarget);
	}
	
	
}

void UUPPCAnimInstance::SetLookAtPos(const FVector& NewLookAtPos)
{
	LookAtPos = NewLookAtPos;
}

FVector UUPPCAnimInstance::GetLookAtPos() const
{
	return LookAtPos;
}

void UUPPCAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	PettuCharacter = Cast<AUPPettuCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), AUPPettuCharacter::StaticClass()));
}
