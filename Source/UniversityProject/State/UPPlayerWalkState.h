// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "State/UPPlayerBaseState.h"
#include "UPPlayerWalkState.generated.h"

/**
 * 
 */
UCLASS()
class UNIVERSITYPROJECT_API UUPPlayerWalkState : public UUPPlayerBaseState
{
	GENERATED_BODY()
	
public:
	UUPPlayerWalkState();

	void Initialize(AUPPlayerCharacter* InOwnerCharacter);

public:
	virtual void EnterState() override;
	virtual void ExitState() override;
	virtual void UpdateState() override;
};
