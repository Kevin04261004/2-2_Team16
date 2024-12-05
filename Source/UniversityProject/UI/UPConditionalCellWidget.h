// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UPUserWidget.h"
#include "UPConditionalCellWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNIVERSITYPROJECT_API UUPConditionalCellWidget : public UUPUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TaskNameText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ProgressText;

	UPROPERTY(meta = (BindWidget))
	class UImage* False;

	UPROPERTY(meta = (BindWidget))
	class UImage* True;
public:
	// 작업 초기화
	void InitializeTask(const FString& TaskName, int32 CurrentProgress, int32 TotalProgress);

	// 진행 상황 업데이트
	void UpdateProgress(int32 CurrentProgress, int32 TotalProgress);
};