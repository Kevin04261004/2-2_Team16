// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/UPBaseAttackableStateInterface.h"
#include "State/UPPlayerBaseSkillState.h"
#include "UPPlayerComboAttackState.generated.h"

/**
 * 
 */
UCLASS()
class UNIVERSITYPROJECT_API UUPPlayerComboAttackState : public UUPPlayerBaseSkillState, public IUPBaseAttackableStateInterface
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
	EPlayerStateType NextAttackStateType;

/* Combo Check Section */
protected:
	// TODO: make this Values to DataAsset;
	float DefaultFrameCount = 30.0f;
	float InputAllowFrameCount = 18.0f;
	float AnimationChangeStartFrameCount = 12.0f;

	UPROPERTY()
	bool bIsAttackKeyDown = false;
	UPROPERTY()
	float OneFrameSec;
	UPROPERTY()
	float CurrentTime;
};
