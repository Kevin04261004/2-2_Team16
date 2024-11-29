#include "Manager/UPSequenceHandler.h"

#include "LevelSequenceActor.h"
#include "Blueprint/UserWidget.h"
#include "Character/UPPlayerCharacter.h"
#include "Components/SlateWrapperTypes.h"
#include "GameFramework/Character.h"

void UUPSequenceHandler::PlaySequence(ULevelSequence* Sequence)
{
	if (!Sequence)
	{
		UE_LOG(LogTemp, Warning, TEXT("Sequence is null!"));
		return;
	}

	FMovieSceneSequencePlaybackSettings PlaybackSettings;
	PlaybackSettings.bAutoPlay = false;
	PlaybackSettings.bHidePlayer = true;
	PlaybackSettings.bDisableMovementInput = true;
	PlaybackSettings.bDisableLookAtInput = true;
	SetSequenceMode();
	
	if (CurSequencePlayer)
	{
		CurSequencePlayer->OnFinished.RemoveAll(this);
	}
	
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

		CurSequencePlayer->OnFinished.AddDynamic(this, &UUPSequenceHandler::EnableCharacterMovement);
		CurSequencePlayer->OnFinished.AddDynamic(this, &UUPSequenceHandler::EnableHiddenUI);
		CurSequencePlayer->OnFinished.AddDynamic(this, &UUPSequenceHandler::EnableInput);
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
	DisableCharacterMovement();
	HideUI();
	DisableInput();

	// TODO: 
}

void UUPSequenceHandler::EnableCharacterMovement()
{
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		AUPPlayerCharacter* PlayerCharacter = Cast<AUPPlayerCharacter>(PC->GetPawn());
		if (PlayerCharacter)
		{
			UCharacterMovementComponent* MovementComponent = PlayerCharacter->GetCharacterMovement();
			if (PlayerCharacter->MovementComponent)
			{
				MovementComponent->SetMovementMode(MOVE_Walking);
			}
			PlayerCharacter->bIsInvincible = false;
			PlayerCharacter->EnableInput(PC);
		}
	}
}


void UUPSequenceHandler::DisableCharacterMovement()
{
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		AUPPlayerCharacter* PlayerCharacter = Cast<AUPPlayerCharacter>(PC->GetPawn());
		PC->FlushPressedKeys(); // 누적된 키 입력을 지움
		PlayerCharacter->DisableInput(PC); // 입력 비활성화
		PlayerCharacter->GetStateManager()->ChangeState(EPlayerStateType::CantMove);
	}
}

void UUPSequenceHandler::HideUI()
{
	for (TObjectIterator<UUserWidget> It; It; ++It)
	{
		if (It->IsInViewport() && It->IsVisible())
		{
			// 현재 화면에 표시되는 위젯만 처리
			It->SetVisibility(ESlateVisibility::Hidden);
			HiddenWidgets.Add(*It); // 숨긴 위젯을 저장
		}
	}

}

void UUPSequenceHandler::EnableHiddenUI()
{
	for (UUserWidget* Widget : HiddenWidgets)
	{
		if (Widget && Widget->IsValidLowLevel())
		{
			Widget->SetVisibility(ESlateVisibility::Visible);
		}
	}

	HiddenWidgets.Empty();
}

void UUPSequenceHandler::EnableInput()
{
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if (APawn* Pawn = PC->GetPawn())
		{
			PC->SetInputMode(FInputModeGameOnly()); // 게임 입력 모드로 전환
			PC->bShowMouseCursor = false;          // 마우스 커서 숨김
			Pawn->EnableInput(PC);
		}
	}
}

void UUPSequenceHandler::DisableInput()
{
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if (APawn* Pawn = PC->GetPawn())
		{
			PC->FlushPressedKeys(); // 이전 입력 초기화
			PC->SetInputMode(FInputModeUIOnly()); // UI 입력 모드로 전환
			PC->bShowMouseCursor = true;         // 마우스 커서 표시
			Pawn->DisableInput(PC);
		}
	}
}
