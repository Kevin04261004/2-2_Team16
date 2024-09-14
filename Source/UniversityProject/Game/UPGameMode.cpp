// Fill out your copyright notice in the Description page of Project Settings.


#include "UPGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "Player/UPPlayerController.h"

AUPGameMode::AUPGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Game/UniversityProject/BluePrints/Character/BP_PlayerCharacter.BP_PlayerCharacter_C"));
	check(DefaultPawnClassRef.Class != nullptr);
	DefaultPawnClass = DefaultPawnClassRef.Class;

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/UniversityProject.UPPlayerController"));
	check(PlayerControllerClassRef.Class != nullptr);
	PlayerControllerClass = PlayerControllerClassRef.Class;

	bIsCleared = false;
}

void AUPGameMode::OnPlayerDead()
{
	AUPPlayerController* PlayerController = Cast<AUPPlayerController>(GetWorld()->GetFirstPlayerController());
	check(PlayerController != nullptr);
	PlayerController->GameOver();
}

bool AUPGameMode::IsGameCleared()
{
	return bIsCleared;
}

void AUPGameMode::BeginPlay()
{
	Super::BeginPlay();

	// 서브레벨 로드
	LoadSublevels();
}

void AUPGameMode::LoadSublevels()
{
	// 서브레벨 리스트
	TArray<FName> SublevelNames = {
		FName("GlobalLevel"),
	};

	for (const FName& SublevelName : SublevelNames)
	{
		UGameplayStatics::LoadStreamLevel(this, SublevelName, true, true, FLatentActionInfo());
	}
}
