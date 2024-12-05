// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TextBlock.h"
#include "UI/UPUserWidget.h"
#include "UPTutorialWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNIVERSITYPROJECT_API UUPTutorialWidget : public UUPUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* ConditionalList;

	// 캔버스 패널
	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* TutorialCanvas;

	// Description 패널
	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* DescriptionCanvas;

	UPROPERTY(meta = (BindWidget))
	class UImage* TutoShowUI;

	bool bTutoShow = true;
	bool bIsAlphaIncreasing = true;
	float CurrentAlpha = 1.0f;
	// 기본 캔버스 크기
	UPROPERTY(EditDefaultsOnly, Category = "Layout")
	FVector2D BaseCanvasSize = FVector2D(500.0f, 0.0f);

	// 작업 하나당 추가될 높이
	UPROPERTY(EditDefaultsOnly, Category = "Layout")
	float TaskHeightIncrement = 50.0f;

	// 작업 위젯 클래스
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUPConditionalCellWidget> CellWidgetClass;

	// 맨 위에 글자들
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TObjectPtr<UTextBlock> DescriptionText;
	
public:
	// 작업 추가
	void AddTask(const FString& TaskName, int32 CurrentProgress, int32 TotalProgress);

	// 특정 작업 진행 상황 업데이트
	void UpdateTask(int32 TaskIndex, int32 CurrentProgress, int32 TotalProgress);

	// 캔버스 크기 업데이트
	void UpdateCanvasSize() const;

	// 작업 초기화
	void ClearAll();

	void SetDescription(const FString& NewDescription);
	void SetTaskVisibleNon() const;

	void TriggerTutoShowUI();
	UPROPERTY()
	FString TargetDescription;

	UPROPERTY()
	FString CurrentTypedText;

	FTimerHandle TypingTimerHandle;
	FTimerHandle AlphaChangeTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	float TypingTime = 0.05f;
	
	void StartTypingEffect(const FString& NewDescription);
	void UpdateTypingEffect();
private:
	// 동적으로 생성된 작업 위젯 목록
	TArray<UUPConditionalCellWidget*> CellWidgets;

	void UpdateAlpha();
};
