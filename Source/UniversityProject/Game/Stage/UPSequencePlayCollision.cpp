// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Stage/UPSequencePlayCollision.h"

#include "Game/UPGameMode.h"
#include "Manager/UPSequenceHandler.h"

class ULevelSequence;
class UUPSequenceHandler;
// Sets default values
AUPSequencePlayCollision::AUPSequencePlayCollision()
{
	CollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CollisionMesh"));
	CollisionMesh->SetGenerateOverlapEvents(true);
	CollisionMesh->OnComponentBeginOverlap.AddDynamic(this, &AUPSequencePlayCollision::OnOverlapBegin);
	Sequence = nullptr;
}

void AUPSequencePlayCollision::OnOverlapBegin(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Sequence == nullptr)
	{
		return;
	}
	
	UUPSequenceHandler* SequenceHandler = GetGameInstance()->GetSubsystem<UUPSequenceHandler>();
	if (SequenceHandler)
	{
		SequenceHandler->PlaySequence(Sequence);
	}


	AUPGameMode* gameMode = Cast<AUPGameMode>(GetWorld()->GetAuthGameMode());
	if (gameMode == nullptr)
	{
		return;
	}
	AUPStageManager* stageManager = gameMode->StageManager;

	if (stageManager == nullptr)
	{
		return;
	}
	stageManager->EvaluateCondition(EStageConditionType::SequencePlay);
	Destroy();
}

