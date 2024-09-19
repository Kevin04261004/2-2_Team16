// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/UPPettuAIController.h"
#include "Character/UPPettuCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AUPPettuAIController::AUPPettuAIController(FObjectInitializer const& ObjectInitializer)
{
	SetupPerceptionSystem();
}

void AUPPettuAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (AUPPettuCharacter* const Pettu = Cast<AUPPettuCharacter>(InPawn))
	{
		if (UBehaviorTree* const tree = Pettu->GetBehaviorTree())
		{
			UBlackboardComponent* b;
			UseBlackboard(tree->BlackboardAsset, b);
			Blackboard = b;
			RunBehaviorTree(tree);
		}
	}
}

void AUPPettuAIController::SetupPerceptionSystem()
{
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	if (SightConfig)
	{
		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
		SightConfig->SightRadius = 500.0f;								 // 감지 반경
		SightConfig->LoseSightRadius = SightConfig->SightRadius + 25.0f; // 시야 잃는 반경
		SightConfig->PeripheralVisionAngleDegrees = 90.0f;				 // 시야 각도
		SightConfig->SetMaxAge(5.0f);									 // 수명
		SightConfig->AutoSuccessRangeFromLastSeenLocation = 520.0f;		 // 자동 성공 범위
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;		 // 적 감지
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;		 // 중립 감지
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;	 // 아군 감지

		GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AUPPettuAIController::OnTargetDetected);
		GetPerceptionComponent()->ConfigureSense(*SightConfig);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Sight Config is not created"));
	}
}

void AUPPettuAIController::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus)
{
}
