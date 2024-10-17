// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

protected:
	UPROPERTY(meta = (BindWidget))
	class USlider* BGMVolumeSlider;
	UPROPERTY(meta = (BindWidget))
	class USlider* SFXVolumeSlider;

	UFUNCTION()
	void OnBGMVolumeChanged(float Value);

	UFUNCTION()
	void OnSFXVolumeChanged(float Value);
};
