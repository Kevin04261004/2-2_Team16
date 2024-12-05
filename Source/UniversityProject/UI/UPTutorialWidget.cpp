// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UPTutorialWidget.h"

#include "UPConditionalCellWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
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

void UUPTutorialWidget::SetDescription(const FString& NewDescription)
{
	if (NewDescription.IsEmpty())
	{
		DescriptionCanvas->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		DescriptionCanvas->SetVisibility(ESlateVisibility::Visible);
		StartTypingEffect(NewDescription);
	}
}

void UUPTutorialWidget::StartTypingEffect(const FString& NewDescription)
{
	// 초기화
	TargetDescription = NewDescription;
	CurrentTypedText = TEXT("");

	// 기존 타이머가 실행 중이면 정지
	if (GetWorld()->GetTimerManager().IsTimerActive(TypingTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(TypingTimerHandle);
	}

	// 타이머 시작 (0.05초 간격으로 실행)
	GetWorld()->GetTimerManager().SetTimer(
		TypingTimerHandle,
		this,
		&UUPTutorialWidget::UpdateTypingEffect,
		TypingTime, // 타이핑 속도 (조정 가능)
		true
	);
}

void UUPTutorialWidget::UpdateTypingEffect()
{
	if (CurrentTypedText.Len() < TargetDescription.Len())
	{
		// 한 글자씩 추가
		CurrentTypedText.AppendChar(TargetDescription[CurrentTypedText.Len()]);
		DescriptionText->SetText(FText::FromString(CurrentTypedText));
	}
	else
	{
		// 모든 텍스트가 출력되면 타이머 정지
		GetWorld()->GetTimerManager().ClearTimer(TypingTimerHandle);
	}
}

void UUPTutorialWidget::SetTaskVisibleNon() const
{
	TutorialCanvas->SetVisibility(ESlateVisibility::Hidden);
}

void UUPTutorialWidget::TriggerTutoShowUI()
{
	bTutoShow = !bTutoShow;
	bIsAlphaIncreasing = bTutoShow; // 켜지면 증가, 꺼지면 감소

	// 타이머 시작: Alpha를 점진적으로 변경
	GetWorld()->GetTimerManager().SetTimer(
		AlphaChangeTimer,
		this,
		&UUPTutorialWidget::UpdateAlpha,
		0.02f, // 업데이트 간격 (20ms)
		true
	);
}

void UUPTutorialWidget::UpdateAlpha()
{
	// Alpha 값 업데이트
	float TargetAlpha = bIsAlphaIncreasing ? 1.0f : 0.0f;
	float Delta = 0.05f; // Alpha 변화 속도

	if (bIsAlphaIncreasing)
	{
		CurrentAlpha = FMath::Min(CurrentAlpha + Delta, TargetAlpha);
	}
	else
	{
		CurrentAlpha = FMath::Max(CurrentAlpha - Delta, TargetAlpha);
	}

	TutoShowUI->SetRenderOpacity(CurrentAlpha);

	if (CurrentAlpha == TargetAlpha)
	{
		GetWorld()->GetTimerManager().ClearTimer(AlphaChangeTimer);
	}
}
