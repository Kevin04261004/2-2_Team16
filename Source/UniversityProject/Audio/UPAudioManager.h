// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UPAudioBaseInfo.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UPAudioManager.generated.h"

UENUM(BlueprintType)
enum class ESFXAudioType : uint8
{
	Hit,
};

UENUM(BlueprintType)
enum class EBGMAudioType : uint8
{
	Title,
	Fight,
};

/**
 * 
 */
UCLASS()
class UNIVERSITYPROJECT_API UUPAudioManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UUPAudioManager();
protected:
	TObjectPtr<class UUPAudioManagerData> AudioManagerData;
public:
	void PlaySoundAtLocation(ESFXAudioType sfx, FVector location);
	void PlaySoundAtLocation(USoundBase* sfx, FVector location);
	void PlayBGM(EBGMAudioType bgm);

private:
	UFUNCTION()
	void OnSFXFinished(UAudioComponent* FinishedComponent);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Audio)
	TArray<UAudioComponent*> ActivatedSFXSounds;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Audio)
	UAudioComponent* ActivatedBGMSound;

public:
	void SetSFXVolume(float volume);
	void SetBGMVolume(float volume);

private:
	float SFXVolume;
	float BGMVolume;
};
