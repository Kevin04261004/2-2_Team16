// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Audio/UPAudioManager.h"
#include "UPAudioManagerData.generated.h"

/**
 * 
 */
UCLASS()
class UNIVERSITYPROJECT_API UUPAudioManagerData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Init)
	TMap<ESFXAudioType, class UUPAudioBaseInfo*> SFXAudioMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Init)
	TMap<EBGMAudioType, class UUPAudioBaseInfo*> BGMAudioMap;
};
