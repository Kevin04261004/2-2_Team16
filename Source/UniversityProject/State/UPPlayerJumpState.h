// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/UPBaseAttackableStateInterface.h"
#include "State/UPPlayerBaseState.h"
#include "UPPlayerJumpState.generated.h"

/**
 * 
 */
UCLASS()
class UNIVERSITYPROJECT_API UUPPlayerJumpState : public UUPPlayerBaseState, public IUPBaseAttackableStateInterface
{
	GENERATED_BODY()
	
public:
	UUPPlayerJumpState();

	virtual void Initialize(AUPPlayerCharacter* InOwnerCharacter, class UUPInputHandlerComponent* InInputHandler) override;

public:
	virtual void EnterState() override;
	virtual void ExitState() override;
	virtual void UpdateState() override;

protected:
	void TryBaseAttack() override;
};
