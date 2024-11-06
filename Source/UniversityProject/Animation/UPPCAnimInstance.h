// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/UPAnimInstance.h"
#include "Character/UPPlayerCharacter.h"
#include "UPPCAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UNIVERSITYPROJECT_API UUPPCAnimInstance : public UUPAnimInstance
{
	GENERATED_BODY()
public:
	UUPPCAnimInstance();
	
	UFUNCTION(BlueprintCallable)
	void UpdateLookAt(float DeltaTime, AUPPlayerCharacter* PlayerCharacter, AUPPettuCharacter* BossCharacter);
	void SetLookAtPos(const FVector& NewLookAtPos);

	UFUNCTION(BlueprintCallable)
	FVector GetLookAtPos() const;

	UFUNCTION(BlueprintCallable)
	AUPPettuCharacter* GetPettuCharacter() const { return PettuCharacter; }

protected:
	virtual void NativeBeginPlay() override;
private:
	FVector LookAtPos;

	AUPPettuCharacter* PettuCharacter;
};
