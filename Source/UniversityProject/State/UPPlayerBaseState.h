// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
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
	void Initialize(AUPPlayerCharacter* InOwnerCharacter);
	
public:
	virtual void EnterState();
	virtual void ExitState();
	virtual void UpdateState();

protected:
	AUPPlayerCharacter* OwnerCharacter;
};
