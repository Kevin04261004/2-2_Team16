// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "State/UPPlayerBaseState.h"
#include "UPPlayerCantMoveState.generated.h"

/**
 * 
 */
UCLASS()
class UNIVERSITYPROJECT_API UUPPlayerCantMoveState : public UUPPlayerBaseState
{
	GENERATED_BODY()
		
public:
	UUPPlayerCantMoveState();

	virtual void Initialize(AUPPlayerCharacter* InOwnerCharacter, class UUPInputHandlerComponent* InInputHandler) override;

public:
	virtual void EnterState() override;
	virtual void ExitState() override;
	virtual void UpdateState() override;
};
