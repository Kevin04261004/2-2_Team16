// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/UPBaseAttackableStateInterface.h"
#include "State/UPPlayerBaseState.h"
#include "UPPlayerInAirState.generated.h"

/**
 * 
 */
UCLASS()
class UNIVERSITYPROJECT_API UUPPlayerInAirState : public UUPPlayerBaseState, public IUPBaseAttackableStateInterface
{
	GENERATED_BODY()
public:
	UUPPlayerInAirState();

	virtual void Initialize(AUPPlayerCharacter* InOwnerCharacter, class UUPInputHandlerComponent* InInputHandler) override;

public:
	virtual void EnterState() override;
	virtual void ExitState() override;
	virtual void UpdateState() override;

protected:
	virtual void TryBaseAttack() override;
};
