// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UPSettingWidget.h"

#include "Audio/UPAudioManager.h"
#include "Components/Slider.h"

void UUPSettingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (BGMVolumeSlider != nullptr)
	{
		BGMVolumeSlider->OnValueChanged.AddDynamic(this, &UUPSettingWidget::OnBGMVolumeChanged);
	}
	
	if (SFXVolumeSlider != nullptr)
	{
		SFXVolumeSlider->OnValueChanged.AddDynamic(this, &UUPSettingWidget::OnSFXVolumeChanged);
	}
}

void UUPSettingWidget::OnBGMVolumeChanged(float Value)
{
	UUPAudioManager* AudioManager = GetGameInstance()->GetSubsystem<UUPAudioManager>();
	if (AudioManager == nullptr)
	{
		return;
	}
	AudioManager->SetBGMVolume(Value);
}

void UUPSettingWidget::OnSFXVolumeChanged(float Value)
{
	UUPAudioManager* AudioManager = GetGameInstance()->GetSubsystem<UUPAudioManager>();
	if (AudioManager == nullptr)
	{
		return;
	}
	AudioManager->SetSFXVolume(Value);
}
