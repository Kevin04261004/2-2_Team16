// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AISenseConfig_Sight.h"
#include "UPPettuAIController.generated.h"

/**
 * 
 */
UCLASS()
class UNIVERSITYPROJECT_API AUPPettuAIController : public AAIController
{
	GENERATED_BODY()
public:
	explicit AUPPettuAIController(FObjectInitializer const& ObjectInitializer);

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY()
	TObjectPtr<UAISenseConfig_Sight> SightConfig;
	void SetupPerceptionSystem();

	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus);
	
};
