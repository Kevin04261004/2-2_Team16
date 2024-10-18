// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/UPPlayerCharacter.h"
#include "UI/UPUserWidget.h"
#include "UPSettingWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNIVERSITYPROJECT_API UUPSettingWidget : public UUPUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

private:
	TObjectPtr<AUPPlayerCharacter> PlayerCharacter;

/* Exit Setting */
protected:
	UPROPERTY(meta = (BindWidget))
	class UButton* ExitSettingButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* ExitGameButton;

	UFUNCTION()
	void OnExitSetting();

	UFUNCTION()
	void OnExitGame();

/* Switch Widget */
protected:
	UPROPERTY(meta = (BindWidget))
	class UButton* SwitchSoundPanelButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* SwitchGraphicPanelButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* SwitchKeySettingPanelButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* SwitchOtherSettingPanelButton;
	
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* SettingWidgetSwitcher;

	UFUNCTION()
	void OnSoundButtonPressed();
	UFUNCTION()
	void OnGraphicButtonPressed();
	UFUNCTION()
	void OnKeySettingButtonPressed();
	UFUNCTION()
	void OnOtherSettingButtonPressed();
	
/* Audio Setting */
protected:
	UPROPERTY(meta = (BindWidget))
	class USlider* BGMVolumeSlider;
	UPROPERTY(meta = (BindWidget))
	class USlider* SFXVolumeSlider;

	UFUNCTION()
	void OnBGMVolumeChanged(float Value);

	UFUNCTION()
	void OnSFXVolumeChanged(float Value);

/* Other Setting */
protected:
	UPROPERTY(meta = (BindWidget))
	class USlider* CameraSpeedSlider;

	UFUNCTION()
	void OnCameraSpeedChanged(float Value);
};
