// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UPTutorialWidget.h"

#include "UPConditionalCellWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"

void UUPTutorialWidget::AddTask(const FString& TaskName, int32 CurrentProgress, int32 TotalProgress)
{
	TutorialCanvas->SetVisibility(ESlateVisibility::HitTestInvisible);
	// 작업 위젯 클래스가 설정되어 있는지 확인
	if (CellWidgetClass == nullptr)
	{
		return;
	}
	// 작업 위젯 생성
	UUPConditionalCellWidget* NewCellWidget = CreateWidget<UUPConditionalCellWidget>(this, CellWidgetClass);
	if (NewCellWidget == nullptr)
	{
		return;
	}
	
	// 작업 내용 초기화
	NewCellWidget->InitializeTask(TaskName, CurrentProgress, TotalProgress);

	// 작업 위젯을 VerticalBox에 추가
	if (ConditionalList)
	{
		UVerticalBoxSlot* NewSlot = ConditionalList->AddChildToVerticalBox(NewCellWidget);
		if (NewSlot)
		{
			NewSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
			NewSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
		}

		// 동적으로 생성된 작업 위젯 목록에 추가
		CellWidgets.Add(NewCellWidget);

		// 캔버스 크기 업데이트
		UpdateCanvasSize();
	}
}

void UUPTutorialWidget::UpdateTask(int32 TaskIndex, int32 CurrentProgress, int32 TotalProgress)
{
	// 유효한 작업 인덱스인지 확인
	if (CellWidgets.IsValidIndex(TaskIndex))
	{
		UUPConditionalCellWidget* TaskWidget = CellWidgets[TaskIndex];
		if (TaskWidget)
		{
			// 진행 상황 업데이트
			TaskWidget->UpdateProgress(CurrentProgress, TotalProgress);
		}
	}
}

void UUPTutorialWidget::UpdateCanvasSize() const
{
	if (!TutorialCanvas) return;

	// 캔버스 크기 계산: 기본 크기 + 작업 개수 * 작업 높이
	FVector2D NewCanvasSize = BaseCanvasSize;
	NewCanvasSize.Y += TaskHeightIncrement * CellWidgets.Num();

	// 캔버스 패널의 슬롯 가져오기
	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(TutorialCanvas->Slot);
	if (CanvasSlot)
	{
		// 크기 업데이트
		CanvasSlot->SetSize(NewCanvasSize);
	}
}

void UUPTutorialWidget::ClearAll()
{
	ConditionalList->ClearChildren();
	CellWidgets.Empty();
}

void UUPTutorialWidget::SetDescription(const FString& NewDescription) const
{
	if (NewDescription.IsEmpty())
	{
		DescriptionCanvas->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		DescriptionCanvas->SetVisibility(ESlateVisibility::Visible);
		DescriptionText->SetText(FText::FromString(NewDescription));	
	}
}

void UUPTutorialWidget::SetTaskVisibleNon() const
{
	TutorialCanvas->SetVisibility(ESlateVisibility::Hidden);
}
