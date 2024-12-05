// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UPConditionalCellWidget.h"

#include "Components/CheckBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UUPConditionalCellWidget::InitializeTask(const FString& TaskName, int32 CurrentProgress, int32 TotalProgress)
{
	if (TaskNameText)
	{
		TaskNameText->SetText(FText::FromString(TaskName));
	}

	if (ProgressText)
	{
		FString ProgressString = FString::Printf(TEXT("( %d / %d )"), CurrentProgress, TotalProgress);
		ProgressText->SetText(FText::FromString(ProgressString));
	}
}

void UUPConditionalCellWidget::UpdateProgress(int32 CurrentProgress, int32 TotalProgress)
{
	if (ProgressText)
	{
		FString ProgressString = FString::Printf(TEXT("( %d / %d )"), CurrentProgress, TotalProgress);
		ProgressText->SetText(FText::FromString(ProgressString));
		if (TotalProgress > CurrentProgress)
		{
			True->SetVisibility(ESlateVisibility::Hidden);
			False->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			True->SetVisibility(ESlateVisibility::Visible);
			False->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}