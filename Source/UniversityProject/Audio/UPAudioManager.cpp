// Fill out your copyright notice in the Description page of Project Settings.


#include "Audio/UPAudioManager.h"

#include "UPAudioBaseInfo.h"
#include "Components/AudioComponent.h"
#include "Audio/UPAudioManagerData.h"

UUPAudioManager::UUPAudioManager()
{
	static ConstructorHelpers::FObjectFinder<UUPAudioManagerData> AudioManagerDataRef(TEXT("/Game/UniversityProject/GameData/Audio/DA_AudioManager.DA_AudioManager"));
	if (AudioManagerDataRef.Succeeded())
	{
		AudioManagerData = AudioManagerDataRef.Object;
	}
}

void UUPAudioManager::PlaySoundAtLocation(ESFXAudioType sfx, FVector location)
{
	if (!AudioManagerData->SFXAudioMap.Contains(sfx))
	{
		return;
	}
	UAudioComponent* createdSound = AudioManagerData->SFXAudioMap[sfx]->PlaySoundAtLocation(GetWorld(), location);
	if (createdSound != nullptr)
	{
		createdSound->OnAudioFinishedNative.AddUObject(this, &UUPAudioManager::OnSFXFinished);

		ActivatedSFXSounds.Add(createdSound);
	}
}

void UUPAudioManager::PlaySoundAtLocation(USoundBase* sfx, FVector location)
{
	UAudioComponent* createdSound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), sfx, location);
	if (createdSound != nullptr)
	{
		createdSound->OnAudioFinishedNative.AddUObject(this, &UUPAudioManager::OnSFXFinished);

		ActivatedSFXSounds.Add(createdSound);
	}
}

void UUPAudioManager::PlayBGM(EBGMAudioType bgm)
{
	if (!AudioManagerData->BGMAudioMap.Contains(bgm) || AudioManagerData->BGMAudioMap[bgm] == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "Playing BGM Failed!!!!!!!");
		return;
	}
	if (ActivatedBGMSound != nullptr)
	{
		ActivatedBGMSound->Stop();
	}
	UAudioComponent* createdSound = AudioManagerData->BGMAudioMap[bgm]->SpawnSound2D(GetWorld());
	if (createdSound != nullptr)
	{
		createdSound->bIsUISound = true;
		ActivatedBGMSound = createdSound;
	}
}

void UUPAudioManager::OnSFXFinished(UAudioComponent* FinishedComponent)
{
	ActivatedSFXSounds.Remove(FinishedComponent);
}

void UUPAudioManager::SetSFXVolume(float volume)
{
	for (UAudioComponent* SFX : ActivatedSFXSounds)
	{
		if (SFX != nullptr)
		{
			SFX->SetVolumeMultiplier(volume);
		}
	}
}

void UUPAudioManager::SetBGMVolume(float volume)
{
	if (ActivatedBGMSound != nullptr)
	{
		ActivatedBGMSound->SetVolumeMultiplier(volume);
	}
}


