// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/UPJumpableStateInterface.h"
#include "State/UPPlayerBaseState.h"
#include "UPPlayerWalkState.generated.h"

/**
 * 
 */
UCLASS()
class UNIVERSITYPROJECT_API UUPPlayerWalkState : public UUPPlayerBaseState, public IUPJumpableStateInterface
{
	GENERATED_BODY()
	
public:
	UUPPlayerWalkState();

	virtual void Initialize(AUPPlayerCharacter* InOwnerCharacter, class UUPInputHandlerComponent* InInputHandler) override;

public:
	virtual void EnterState() override;
	virtual void ExitState() override;
	virtual void UpdateState() override;
	
protected:
	virtual void TryJump() override;
};
