// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Stage/UPStageManager.h"

#include "Game/UPGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/UPActorSpawner.h"
#include "UI/UPTutorialWidget.h"

// Sets default values
AUPStageManager::AUPStageManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AUPStageManager::BeginPlay()
{
	Super::BeginPlay();

	FString CurrentLevelName = GetWorld()->GetMapName(); // 전체 이름: /Game/Maps/TitleLevel
	CurrentLevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix); // 경로 및 접두사를 제거해 레벨 이름만 남김

	if (CurrentLevelName == "TitleLevel")
	{
		// TitleLevel이면 실행 중단
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("Current level is TitleLevel. Skipping BeginPlay logic."));
		return;
	}
	
	InitializeTutorialWidget();
	
	UUPActorSpawner* ActorSpawner = GetGameInstance()->GetSubsystem<UUPActorSpawner>();
	if (ActorSpawner != nullptr)
	{
		ActorSpawner->InitializeSpawner(this);
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "Actor Spawner Initialized");
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Actor Spawner Not Found");
	}
}

void AUPStageManager::InitializeTutorialWidget()
{
	if (TutorialWidgetClass == nullptr)
	{
		return;
	}
	// 위젯 생성
	TutorialWidget = CreateWidget<UUPTutorialWidget>(GetWorld(), TutorialWidgetClass);
	if (TutorialWidget)
	{
		TutorialWidget->SetVisibility(ESlateVisibility::Hidden);

		// Focusable 비활성화
		TutorialWidget->SetIsFocusable(false);
		// 화면에 표시
		TutorialWidget->AddToViewport(-10);
	}
	AUPGameMode* UPGameMode = Cast<AUPGameMode>(GetWorld()->GetAuthGameMode());
	check(UPGameMode != nullptr);
	HudWidget = UPGameMode->HudWidgetObject;
	check(HudWidget != nullptr);
}

void AUPStageManager::EvaluateCondition(EStageConditionType ConditionType)
{
	if (StageTutorialData->TutorialStages.IsValidIndex(CurrentStageIndex))
	{
		if (CurrentStage.StageConditionMap.Find(ConditionType))
		{
			CurrentStage.StageConditionMap[ConditionType]--;
			
			if (TutorialWidget)
			{
				int32 TaskIndex = 0;
				for (const TPair<EStageConditionType, int32>& Condition : CurrentStage.StageConditionMap)
				{
					if (Condition.Key == ConditionType)
					{
						int32 max = StageTutorialData->TutorialStages[CurrentStageIndex].StageConditionMap[Condition.Key];
						int32 remaining = max - Condition.Value;
						TutorialWidget->UpdateTask(TaskIndex, remaining, max);
						break;
					}
					TaskIndex++;
				}
			}
			CheckStageConditions();
		}
	}
}

void AUPStageManager::TutorialStartStage(int32 StageIndex)
{
	if (StageTutorialData == nullptr)
	{
		return;
	}
	TutorialWidget->SetIsEnabled(true);
	TutorialWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
	CurrentStageIndex = StageIndex;
	if (StageTutorialData->TutorialStages.IsValidIndex(CurrentStageIndex))
	{
		CurrentStage = StageTutorialData->TutorialStages[CurrentStageIndex];

		if (TutorialWidget)
		{
			TutorialWidget->ClearAll(); // 이전 조건 초기화

			TutorialWidget->SetDescription(CurrentStage.Description);

			HudWidget->SetExpression(CurrentStage.ExpressionType);

			if (CurrentStage.StageConditionMap.Num() == 0)
			{
				TutorialWidget->SetTaskVisibleNon();
			}
			for (const TPair<EStageConditionType, int32>& Condition : CurrentStage.StageConditionMap)
			{
				TutorialWidget->AddTask(StageTutorialData->TutorialConditionDescriptionMap[Condition.Key], 0, Condition.Value);
			}
		}
		if (CurrentStage.SpawnActorKey.Len() > 0)
		{
			OnStageStart.Broadcast(CurrentStage.SpawnActorKey);
		}
		if (CurrentStage.bIsDialogue)
		{
			CompleteStage();
		}
	}
	else
	{
		TutorialStageClear();
	}
}

void AUPStageManager::CompleteStage()
{
	OnStageClear.Broadcast(CurrentStage.SpawnActorKey);
	
	if (StageTutorialData->TutorialStages.IsValidIndex(CurrentStageIndex))
	{
		if (CurrentStage.TurmAfterClear <= 0.0f)
		{
			CurrentStage.TurmAfterClear = 0.1f;
		}
		GetWorld()->GetTimerManager().SetTimer(
			NextStageTimerHandle,
			this,
			&AUPStageManager::StartNextStage,
			CurrentStage.TurmAfterClear,
			false
		);
	}
}

void AUPStageManager::StartNextStage()
{
	CurrentStageIndex++;
	TutorialStartStage(CurrentStageIndex);
}

void AUPStageManager::SkipTutorial()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(NextStageTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(NextStageTimerHandle);
	}
	TutorialStageClear();
}

void AUPStageManager::TutorialStageClear()
{
	AUPGameMode* GameMode = Cast<AUPGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		GameMode->OnTutorialClear();
	}

	TutorialWidget->SetIsEnabled(false);
	TutorialWidget->SetVisibility(ESlateVisibility::Hidden);

	OnTutorialStageClear.Broadcast();
	
	BossStageStart();
}

void AUPStageManager::CheckStageConditions()
{
	bool bAllConditionsMet = true;

	for (const TPair<EStageConditionType, int>& Condition : CurrentStage.StageConditionMap)
	{
		if (Condition.Value > 0)
		{
			bAllConditionsMet = false;
			break;
		}
	}

	if (bAllConditionsMet)
	{
		CompleteStage();
	}
}

void AUPStageManager::BossStageStart()
{
	OnBossStageStart.Broadcast();
}
