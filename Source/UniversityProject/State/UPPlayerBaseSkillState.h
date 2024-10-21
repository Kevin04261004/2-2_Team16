// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/UPDashableStateInterface.h"
#include "State/UPPlayerBaseState.h"
#include "UPPlayerBaseSkillState.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class UNIVERSITYPROJECT_API UUPPlayerBaseSkillState : public UUPPlayerBaseState, public IUPDashableStateInterface
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
	virtual void SkillFinished();
	virtual void TryDash() override;
	float SkillDuration;
	FTimerHandle SkillEndTimerHandle;

/* Combo Check Section */
protected:
	// TODO: make this Values to DataAsset;
	TObjectPtr<UUPComboInputableData> ComboInputableData;

	UPROPERTY()
	bool bIsAttackKeyDown = false;
	UPROPERTY()
	bool bIsDashKeyDown = false;
	UPROPERTY()
	float OneFrameSec = 0.f;
	UPROPERTY()
	float CurrentTime;
	UPROPERTY()
	float ComboAnimationStartTime;
};
