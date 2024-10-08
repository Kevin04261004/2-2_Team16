// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "State/UPPlayerBaseState.h"
#include "UPPlayerBaseSkillState.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class UNIVERSITYPROJECT_API UUPPlayerBaseSkillState : public UUPPlayerBaseState
{
	GENERATED_BODY()
public:
	UUPPlayerBaseSkillState();

	virtual void Initialize(AUPPlayerCharacter* InOwnerCharacter, class UUPInputHandlerComponent* InInputHandler) override;

public:
	virtual void InitSkillData();
	virtual void EnterState() override;
	virtual void ExitState() override;
	virtual void UpdateState() override;

protected:
	EPlayerSkillType ThisSkillType;
	
	UFUNCTION()
	virtual void SkillFinished();
	
	FTimerHandle SkillEndTimerHandle;
};
