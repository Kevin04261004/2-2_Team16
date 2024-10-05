// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "State/UPPlayerBaseState.h"
#include "UPPlayerRunState.generated.h"

/**
 * 
 */
UCLASS()
class UNIVERSITYPROJECT_API UUPPlayerRunState : public UUPPlayerBaseState
{
	GENERATED_BODY()
		
public:
	UUPPlayerRunState();

	void Initialize(AUPPlayerCharacter* InOwnerCharacter, class UUPInputHandlerComponent* InInputHandler);

public:
	virtual void EnterState() override;
	virtual void ExitState() override;
	virtual void UpdateState() override;
};
