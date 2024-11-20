// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Stage/UPReachLocationCollision.h"

#include "UPStageManager.h"
#include "Character/UPPlayerCharacter.h"
#include "Game/UPGameMode.h"

// Sets default values
AUPReachLocationCollision::AUPReachLocationCollision()
{
	CylinderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CylinderMesh"));
	CylinderMesh->SetGenerateOverlapEvents(true);
	CylinderMesh->OnComponentBeginOverlap.AddDynamic(this, &AUPReachLocationCollision::OnOverlapBegin);
}

void AUPReachLocationCollision::OnOverlapBegin(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == nullptr)
	{
		return;
	}
	AUPPlayerCharacter* PlayerCharacter = Cast<AUPPlayerCharacter>(OtherActor);
	if (PlayerCharacter == nullptr)
	{
		return;
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
	
	// TODO: 이펙트 추가
	
	Destroy();
}
