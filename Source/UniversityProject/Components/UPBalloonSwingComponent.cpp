#include "Components/UPBalloonSwingComponent.h"

UUPBalloonSwingComponent::UUPBalloonSwingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UUPBalloonSwingComponent::BeginPlay()
{
	Super::BeginPlay();
	RunningTime = FMath::FRand() * 100.0f;
}


// Called every frame
void UUPBalloonSwingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (AActor* Owner = GetOwner())
	{
		RunningTime += DeltaTime;

		float Yaw = FMath::Sin(RunningTime * SwingSpeed) * SwingAmplitudeYaw;

		float Roll = FMath::Cos(RunningTime * SwingSpeed * 0.8f) * SwingAmplitudeRoll;

		// 난수
		Yaw += FMath::PerlinNoise1D(RunningTime * 0.5f) * RandomNoiseIntensity;
		Roll += FMath::PerlinNoise1D((RunningTime + 50.0f) * 0.5f) * RandomNoiseIntensity;

		FRotator NewRotation = FRotator(0.0f, Yaw, Roll);
		Owner->SetActorRelativeRotation(NewRotation);
	}
}

