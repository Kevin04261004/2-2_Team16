// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UPPlayerBaseSkillState.h"
#include "Interface/UPDashableStateInterface.h"
#include "State/UPPlayerBaseState.h"
#include "UPPlayerTakeDownState.generated.h"

/**
 * 
 */
UCLASS()
class UNIVERSITYPROJECT_API UUPPlayerTakeDownState : public UUPPlayerBaseSkillState, public IUPDashableStateInterface
{
	GENERATED_BODY()
		
public:
	UUPPlayerTakeDownState();

	virtual void Initialize(AUPPlayerCharacter* InOwnerCharacter, class UUPInputHandlerComponent* InInputHandler) override;

public:
	virtual void InitSkillData() override;
	virtual void EnterState() override;
	virtual void ExitState() override;
	virtual void UpdateState() override;

	void GoUp();
protected:
	virtual void TryDash() override;
	virtual void SkillFinished() override;
};
