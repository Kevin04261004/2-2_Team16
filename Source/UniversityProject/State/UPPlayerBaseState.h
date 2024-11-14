// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "UPStateManager.h"
#include "Character/UPPlayerCharacter.h"
#include "UPPlayerBaseState.generated.h"

class AUPPlayerCharacter;

/**
 * 
 */
UCLASS(Abstract)
class UNIVERSITYPROJECT_API UUPPlayerBaseState : public UObject
{
	GENERATED_BODY()

public:
	UUPPlayerBaseState();
	virtual void Initialize(AUPPlayerCharacter* InOwnerCharacter, class UUPInputHandlerComponent* InInputHandler);
	
public:
	virtual void EnterState();
	virtual void ExitState();
	virtual void UpdateState();

protected:
	TObjectPtr<AUPPlayerCharacter> OwnerCharacter;
	TObjectPtr<UUPInputHandlerComponent> InputHandler;
	void TakeDamaged(float amount);

protected:
	FORCEINLINE void ChangeState(EPlayerStateType NextState) const { OwnerCharacter->GetStateManager()->ChangeState(NextState); }
};
