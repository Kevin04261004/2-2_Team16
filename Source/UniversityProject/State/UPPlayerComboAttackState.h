// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameData/UPComboInputableData.h"
#include "Interface/UPBaseAttackableStateInterface.h"
#include "Interface/UPUppercutableStateInterface.h"
#include "State/UPPlayerBaseSkillState.h"
#include "UPPlayerComboAttackState.generated.h"

/**
 * 
 */
UCLASS()
class UNIVERSITYPROJECT_API UUPPlayerComboAttackState : public UUPPlayerBaseSkillState, public IUPBaseAttackableStateInterface, public IUPUppercutableStateInterface
{
	GENERATED_BODY()
		
public:
	UUPPlayerComboAttackState();

	virtual void Initialize(AUPPlayerCharacter* InOwnerCharacter, class UUPInputHandlerComponent* InInputHandler) override;

public:
	virtual void InitSkillData() override;
	virtual void EnterState() override;
	virtual void ExitState() override;
	virtual void UpdateState() override;
	
protected:
	virtual void SkillFinished() override;
	virtual void TryBaseAttack() override;
	virtual void TryUpperCut() override;
	EPlayerStateType NextAttackStateType;
};
