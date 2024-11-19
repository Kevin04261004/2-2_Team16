#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "MovieSceneSequencePlaybackSettings.h"
#include "UPSequenceHandler.generated.h"

UCLASS()
class YOURGAME_API UUPSequenceHandler : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// 시퀀스 재생 함수
	UFUNCTION(BlueprintCallable, Category = "Cinematic")
	void PlaySequence(ULevelSequence* Sequence);

	// 시퀀스 제어 함수들
	UFUNCTION(BlueprintCallable, Category = "Cinematic")
	void PauseSequence();

	UFUNCTION(BlueprintCallable, Category = "Cinematic")
	void SkipSequence();

	UFUNCTION(BlueprintCallable, Category = "Cinematic")
	void SetSequenceMode();

private:
	// 현재 재생 중인 시퀀스 플레이어
	UPROPERTY()
	ULevelSequencePlayer* CurSequencePlayer;

	// 현재 시퀀스 액터 (필요한 경우 추가 설정 가능)
	UPROPERTY()
	ALevelSequenceActor* CurSequenceActor;
};
