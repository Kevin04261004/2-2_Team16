// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UPPlayerBaseSkillState.h"
#include "State/UPPlayerBaseState.h"
#include "UPPlayerDashState.generated.h"

/**
 * 
 */
UCLASS()
class UNIVERSITYPROJECT_API UUPPlayerDashState : public UUPPlayerBaseSkillState
{
	GENERATED_BODY()
	
public:
	UUPPlayerDashState();

	virtual void Initialize(AUPPlayerCharacter* InOwnerCharacter, class UUPInputHandlerComponent* InInputHandler) override;

public:
	virtual void InitSkillData() override;
	virtual void EnterState() override;
	virtual void ExitState() override;
	virtual void UpdateState() override;

protected:
	virtual void SkillFinished() override;
};
