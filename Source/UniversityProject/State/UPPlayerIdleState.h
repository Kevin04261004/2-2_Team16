// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/UPBaseAttackableStateInterface.h"
#include "Interface/UPDashableStateInterface.h"
#include "Interface/UPJumpableStateInterface.h"
#include "Interface/UPUppercutableStateInterface.h"
#include "State/UPPlayerBaseState.h"
#include "UPPlayerIdleState.generated.h"

/**
 * 
 */
UCLASS()
class UNIVERSITYPROJECT_API UUPPlayerIdleState : public UUPPlayerBaseState, public IUPJumpableStateInterface, public IUPDashableStateInterface, public IUPBaseAttackableStateInterface, public IUPUppercutableStateInterface
{
	GENERATED_BODY()

public:
	UUPPlayerIdleState();

	virtual void Initialize(AUPPlayerCharacter* InOwnerCharacter, class UUPInputHandlerComponent* InInputHandler) override;

public:
	virtual void EnterState() override;
	virtual void ExitState() override;
	virtual void UpdateState() override;

protected:
	virtual void TryJump() override;
	virtual void TryDash() override;
	virtual void TryBaseAttack() override;
	virtual void TryUpperCut() override;
};
