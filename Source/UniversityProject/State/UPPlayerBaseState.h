// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/UPPlayerCharacter.h"
#include "UPPlayerBaseState.generated.h"

/**
 * 
 */
UCLASS()
class UNIVERSITYPROJECT_API UUPPlayerBaseState : public UObject
{
	GENERATED_BODY()

public:
	UUPPlayerBaseState();

public:
	virtual void EnterState(AUPPlayerCharacter* OwnerCharacter) {}
	virtual void ExitState(AUPPlayerCharacter* OwnerCharacter) {}
	virtual void HandleInput(AUPPlayerCharacter* OwnerCharacter) {}
};
