// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/UPPettuAIController.h"
#include "Character/Enemy/UPPettuCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/UPPlayerCharacter.h"
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
		SightConfig->PeripheralVisionAngleDegrees = 360.0f;				 // 시야 각도
		SightConfig->SetMaxAge(5.0f);									 // 감지되고 5초동안은 감지된 정보를 기억
		SightConfig->AutoSuccessRangeFromLastSeenLocation = 520.0f;		 // 마지막으로 본 위치로부터 반경 내에 있는 목표를 감지한 것으로 간주
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;		 // 적 감지
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;		 // 중립 감지
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;	 // 아군 감지
		
		GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation()); // 시각 감지를 우선으로 설정
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AUPPettuAIController::OnTargetDetected); // 감지된 대상이 업데이트 될 때마다 호출
		GetPerceptionComponent()->ConfigureSense(*SightConfig); // 감지 설정 적용
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Sight Config is not created"));
	}
}

void AUPPettuAIController::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus)
{
	if (auto* const Ch = Cast<AUPPlayerCharacter>(Actor))
	{
		GetBlackboardComponent()->SetValueAsBool("CanSeePlayer", Stimulus.WasSuccessfullySensed());
	}
}
