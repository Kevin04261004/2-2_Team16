// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/NoExportTypes.h"
#include "UPAudioBaseInfo.generated.h"

/**
 * 
 */
UCLASS()
class UNIVERSITYPROJECT_API UUPAudioBaseInfo : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP-Sound", meta = (AllowPrivateAccess = true))
	TObjectPtr<USoundBase> Sound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP-Sound", meta = (AllowPrivateAccess = true))
	float Pitch = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP-Sound", meta = (AllowPrivateAccess = true))
	float Volume = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP-Sound", meta = (AllowPrivateAccess = true))
	float StartTime = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP-Sound", meta = (AllowPrivateAccess = true))
	TObjectPtr<USoundAttenuation> SoundAttenuation;

public:
	UAudioComponent* PlaySoundAtLocation(const UObject* WorldContextObject, FVector Location);

	UAudioComponent* SpawnSound2D(const UObject* WorldContextObject);
};
