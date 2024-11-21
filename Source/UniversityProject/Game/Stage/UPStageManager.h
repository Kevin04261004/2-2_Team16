// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UPTutorialStage.h"
#include "GameData/UPStageTutorialData.h"
#include "GameFramework/Actor.h"
#include "UPStageManager.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnStageClear, FString /* Spawn Actor Key */)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnStageConditionUpdate, FString /* Spawn Actor Key */)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnStageStart, FString /* Spawn Actor Key */)
DECLARE_MULTICAST_DELEGATE(FOnTutorialStageClear)

UCLASS()
class UNIVERSITYPROJECT_API AUPStageManager : public AActor
{
	GENERATED_BODY()
	
public:
	AUPStageManager();

	virtual void BeginPlay() override;

private:
	// 현재 스테이지 데이터를 표시할 위젯
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UUPTutorialWidget> TutorialWidgetClass;

	// 현재 생성된 위젯
	UPROPERTY()
	UUPTutorialWidget* TutorialWidget;

	// 위젯 초기화
	void InitializeTutorialWidget();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Init")
	TObjectPtr<UUPStageTutorialData> StageTutorialData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 CurrentStageIndex = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FUPTutorialStage CurrentStage;
	
	void EvaluateCondition(EStageConditionType ConditionType);

	FOnStageClear OnStageClear;
	FOnStageConditionUpdate OnStageConditionUpdate;
	FOnStageStart OnStageStart;
	FOnTutorialStageClear OnTutorialStageClear;

	FTimerHandle NextStageTimerHandle;

	UFUNCTION()
	void SkipTutorial();
private:
	void TutorialStartStage(int32 StageIndex);
	void CompleteStage();
	void StartNextStage();
	void TutorialStageClear();
	void CheckStageConditions();
};

