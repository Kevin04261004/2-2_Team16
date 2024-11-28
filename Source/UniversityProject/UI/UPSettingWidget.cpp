// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UPSettingWidget.h"

#include "EngineUtils.h"
#include "Audio/UPAudioManager.h"
#include "Character/UPPlayerCharacter.h"
#include "Components/Button.h"
#include "Components/Slider.h"
#include "Components/UPCameraComponent.h"
#include "Components/WidgetSwitcher.h"
#include "Player/UPPlayerController.h"

void UUPSettingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (PlayerCharacter == nullptr)
	{
		FindPlayerInWorld();
	}

	/* Sound */
	if (BGMVolumeSlider != nullptr)
	{
		BGMVolumeSlider->OnValueChanged.AddDynamic(this, &UUPSettingWidget::OnBGMVolumeChanged);
	}
	
	if (SFXVolumeSlider != nullptr)
	{
		SFXVolumeSlider->OnValueChanged.AddDynamic(this, &UUPSettingWidget::OnSFXVolumeChanged);
	}

	/* Exit */
	if (ExitSettingButton != nullptr)
	{
		ExitSettingButton->OnClicked.AddDynamic(this, &UUPSettingWidget::OnExitSetting);
	}
	
	if (ExitGameButton != nullptr)
	{
		ExitGameButton->OnClicked.AddDynamic(this, &UUPSettingWidget::OnExitGame);
	}

	/* Switch */
	if (SwitchSoundPanelButton != nullptr)
	{
		SwitchSoundPanelButton->OnClicked.AddDynamic(this, &UUPSettingWidget::OnSoundButtonPressed);
	}
	if (SwitchGraphicPanelButton != nullptr)
	{
		SwitchGraphicPanelButton->OnClicked.AddDynamic(this, &UUPSettingWidget::OnGraphicButtonPressed);
	}
	if (SwitchKeySettingPanelButton != nullptr)
	{
		SwitchKeySettingPanelButton->OnClicked.AddDynamic(this, &UUPSettingWidget::OnKeySettingButtonPressed);
	}
	if (SwitchOtherSettingPanelButton != nullptr)
	{
		SwitchOtherSettingPanelButton->OnClicked.AddDynamic(this, &UUPSettingWidget::OnOtherSettingButtonPressed);
	}

	/* Other */
	if (CameraSpeedSlider != nullptr)
	{
		CameraSpeedSlider->OnValueChanged.AddDynamic(this, &UUPSettingWidget::OnCameraSpeedChanged);
	}
}

void UUPSettingWidget::OnExitSetting()
{
	AUPPlayerController* PlayerController = Cast<AUPPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController == nullptr)
	{
		return;
	}
	
	PlayerController->SetGameMode();
}

void UUPSettingWidget::OnExitGame()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController == nullptr)
	{
		return;
	}
	UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, false);
}

void UUPSettingWidget::OnSoundButtonPressed()
{
	SettingWidgetSwitcher->SetActiveWidgetIndex(0);
}

void UUPSettingWidget::OnGraphicButtonPressed()
{
	SettingWidgetSwitcher->SetActiveWidgetIndex(1);
}

void UUPSettingWidget::OnKeySettingButtonPressed()
{
	SettingWidgetSwitcher->SetActiveWidgetIndex(2);
}

void UUPSettingWidget::OnOtherSettingButtonPressed()
{
	SettingWidgetSwitcher->SetActiveWidgetIndex(3);
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

void UUPSettingWidget::OnCameraSpeedChanged(float Value)
{
	if (PlayerCharacter == nullptr)
	{
		FindPlayerInWorld();
	}
	if (PlayerCharacter == nullptr)
	{
		return;
	}
	PlayerCharacter->GetCameraComponent()->CameraSpeed = Value;
}

void UUPSettingWidget::FindPlayerInWorld()
{
	for (TActorIterator<AUPPlayerCharacter> It(GetWorld()); It; ++It)
	{
		AUPPlayerCharacter* player = *It;
		if (player != nullptr)
		{
			PlayerCharacter = player;
			break;
		}
	}
}
