#include "Manager/UPSequenceHandler.h"

void UUPSequenceHandler::PlaySequence(ULevelSequence* Sequence)
{
	if (!Sequence)
	{
		UE_LOG(LogTemp, Warning, TEXT("Sequence is null!"));
		return;
	}

	FMovieSceneSequencePlaybackSettings PlaybackSettings;
	PlaybackSettings.bAutoPlay = false;

	// ULevelSequencePlayer 생성
	CurSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
		GetWorld(),
		Sequence,
		PlaybackSettings,
		CurSequenceActor
	);

	if (CurSequencePlayer)
	{
		UE_LOG(LogTemp, Log, TEXT("Playing Level Sequence..."));
		CurSequencePlayer->Play();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create Level Sequence Player!"));
	}
}

void UUPSequenceHandler::PauseSequence()
{
	if (CurSequencePlayer)
	{
		CurSequencePlayer->Pause();
		UE_LOG(LogTemp, Log, TEXT("Sequence Paused."));
	}
}

void UUPSequenceHandler::SkipSequence()
{
	if (CurSequencePlayer)
	{
		float SequenceDuration = CurSequencePlayer->GetDuration().AsSeconds();

		// 재생 시간을 끝으로 이동
		FMovieSceneSequencePlaybackParams PlaybackParams = FMovieSceneSequencePlaybackParams(SequenceDuration, EUpdatePositionMethod::Jump);
		CurSequencePlayer->SetPlaybackPosition(PlaybackParams);
		
		UE_LOG(LogTemp, Log, TEXT("Sequence Skipped to End."));
	}
}

void UUPSequenceHandler::SetSequenceMode()
{
	// 여기에 추가적인 모드 설정 로직을 구현
	UE_LOG(LogTemp, Log, TEXT("Sequence mode set."));
}
