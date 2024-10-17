// Fill out your copyright notice in the Description page of Project Settings.


#include "SetBGMOneTime.h"

#include "Audio/UPAudioManager.h"

// Sets default values
ASetBGMOneTime::ASetBGMOneTime()
{
	
}

void ASetBGMOneTime::BeginPlay()
{
	Super::BeginPlay();

	UUPAudioManager* AudioManager = GetGameInstance()->GetSubsystem<UUPAudioManager>();
	if (AudioManager != nullptr)
	{
		AudioManager->PlayBGM(SceneBGM);
	}
}


