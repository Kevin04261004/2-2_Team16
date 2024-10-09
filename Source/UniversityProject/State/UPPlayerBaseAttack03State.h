// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "State/UPPlayerComboAttackState.h"
#include "UPPlayerBaseAttack03State.generated.h"

/**
 * 
 */
UCLASS()
class UNIVERSITYPROJECT_API UUPPlayerBaseAttack03State : public UUPPlayerComboAttackState
{
	GENERATED_BODY()

public:
	UUPPlayerBaseAttack03State();

	virtual void Initialize(AUPPlayerCharacter* InOwnerCharacter, class UUPInputHandlerComponent* InInputHandler) override;

public:
	virtual void InitSkillData() override;
	virtual void EnterState() override;
	virtual void ExitState() override;
	virtual void UpdateState() override;

protected:
	virtual void SkillFinished() override;
};
