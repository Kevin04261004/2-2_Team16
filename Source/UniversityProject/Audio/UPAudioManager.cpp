// Fill out your copyright notice in the Description page of Project Settings.


#include "Audio/UPAudioManager.h"

#include "EngineUtils.h"
#include "UPAudioBaseInfo.h"
#include "Components/AudioComponent.h"
#include "Audio/UPAudioManagerData.h"
#include "Sound/AmbientSound.h"

class AAmbientSound;

UUPAudioManager::UUPAudioManager()
{
	static ConstructorHelpers::FObjectFinder<UUPAudioManagerData> AudioManagerDataRef(TEXT("/Game/UniversityProject/GameData/Audio/DA_AudioManager.DA_AudioManager"));
	if (AudioManagerDataRef.Succeeded())
	{
		AudioManagerData = AudioManagerDataRef.Object;
	}

	SFXVolume = 0.5f;
	BGMVolume = 0.5f;
}

void UUPAudioManager::PlaySoundAtLocation(ESFXAudioType sfx, FVector location)
{
	if (!AudioManagerData->SFXAudioMap.Contains(sfx) || AudioManagerData->SFXAudioMap[sfx] == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "Playing SFX Failed!!!!!!!");
		return;
	}
	PlaySoundAtLocation(AudioManagerData->SFXAudioMap[sfx]->Sound, location);
}


void UUPAudioManager::PlaySoundAtLocation(USoundBase* sfx, FVector location)
{
	UAudioComponent* createdSound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), sfx, location, FRotator::ZeroRotator, SFXVolume);
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
		createdSound->SetVolumeMultiplier(BGMVolume);
		ActivatedBGMSound = createdSound;
	}
}

void UUPAudioManager::OnSFXFinished(UAudioComponent* FinishedComponent)
{
	ActivatedSFXSounds.Remove(FinishedComponent);
}

void UUPAudioManager::SetSFXVolume(float volume)
{
	SFXVolume = volume;

	for (UAudioComponent* SFX : ActivatedSFXSounds)
	{
		if (SFX != nullptr)
		{
			SFX->SetVolumeMultiplier(SFXVolume);
		}
	}
}

void UUPAudioManager::SetBGMVolume(float volume)
{
	BGMVolume = volume;

	if (ActivatedBGMSound != nullptr)
	{
		if (FMath::IsNearlyZero(BGMVolume)) 
		{
			ActivatedBGMSound->SetPaused(true);
		}
		else 
		{
			if (ActivatedBGMSound->bIsPaused)
			{
				ActivatedBGMSound->SetPaused(false);
			}
			ActivatedBGMSound->SetVolumeMultiplier(BGMVolume);
		}
	}
}

void UUPAudioManager::CollectAllSounds(UWorld* World)
{
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("World is null. Cannot collect AmbientSounds."));
		return;
	}

	// 월드 내 모든 AmbientSound 액터 순회
	for (TActorIterator<AAmbientSound> ActorItr(World); ActorItr; ++ActorItr)
	{
		AAmbientSound* AmbientSound = *ActorItr;
		if (AmbientSound && AmbientSound->GetAudioComponent())
		{
			UAudioComponent* AudioComponent = AmbientSound->GetAudioComponent();
			if (AudioComponent && AudioComponent->Sound)
			{
				// AmbientSound의 사운드 추가
				ActivatedSFXSounds.Add(AudioComponent);
				UE_LOG(LogTemp, Log, TEXT("Found AmbientSound: %s"), *AudioComponent->Sound->GetName());
			}
		}
	}
	UE_LOG(LogTemp, Log, TEXT("Collected %d sounds."), ActivatedSFXSounds.Num());
}
