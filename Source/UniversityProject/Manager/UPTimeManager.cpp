// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/UPTimeManager.h"

#include "Kismet/GameplayStatics.h"

void UUPTimeManager::WorldTimeUp()
{
	timeScale += 0.1f;
	SetGlobalTimeDilation(timeScale);
}

void UUPTimeManager::WorldTimeDown()
{
	timeScale -= 0.1f;
	if (timeScale < 0.1f)
	{
		timeScale = 0.1f;
	}
	SetGlobalTimeDilation(timeScale);
}

void UUPTimeManager::WorldTimeReset()
{
	timeScale = 1.0f;
	SetGlobalTimeDilation(timeScale);
}

void UUPTimeManager::WorldTimeStop()
{
	timeScale = 0.0f;
	SetGlobalTimeDilation(timeScale);
}

void UUPTimeManager::SetGlobalTimeDilation(float NewTimeScale)
{
	// Unreal Engine의 전역 시간 흐름 변경
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), NewTimeScale);
}
