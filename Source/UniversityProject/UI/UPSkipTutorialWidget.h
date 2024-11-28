// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UPUserWidget.h"
#include "UPSkipTutorialWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNIVERSITYPROJECT_API UUPSkipTutorialWidget : public UUPUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
/* Skip Widget */
protected:
	UPROPERTY(meta = (BindWidget))
	class UButton* SkipTutorialButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* NonSkipTutorialButton;

	UFUNCTION()
	void OnSkipTutorialButtonPressed();
	UFUNCTION()
	void OnNonSkipTutorialButtonPressed();
};
