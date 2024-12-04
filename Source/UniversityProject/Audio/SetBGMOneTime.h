// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Audio/UPAudioManager.h"
#include "SetBGMOneTime.generated.h"

UCLASS()
class UNIVERSITYPROJECT_API ASetBGMOneTime : public AActor
{
	GENERATED_BODY()
	
public:
	ASetBGMOneTime();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Init", meta = (AllowPrivateAccess = "true", ToolTip = "현재 씬의 Type을 선택해주세요."))
	EBGMAudioType SceneBGM;
	
	virtual void BeginPlay() override;
};
