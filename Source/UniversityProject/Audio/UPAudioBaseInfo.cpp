// Fill out your copyright notice in the Description page of Project Settings.


#include "Audio/UPAudioBaseInfo.h"

UAudioComponent* UUPAudioBaseInfo::PlaySoundAtLocation(const UObject* WorldContextObject, FVector Location)
{
	UAudioComponent* AudioComponent = nullptr;
	if (Sound != nullptr)
	{
		AudioComponent = UGameplayStatics::SpawnSoundAtLocation(WorldContextObject, Sound, Location, FRotator::ZeroRotator, Volume, Pitch, StartTime, SoundAttenuation);
	}

	return AudioComponent;
}

UAudioComponent* UUPAudioBaseInfo::SpawnSound2D(const UObject* WorldContextObject)
{
	UAudioComponent* AudioComponent = nullptr;
	if (Sound != nullptr)
	{
		AudioComponent = UGameplayStatics::SpawnSound2D(WorldContextObject, Sound, Volume, Pitch, StartTime, nullptr, false, false);
	}
	
	return AudioComponent;
}
