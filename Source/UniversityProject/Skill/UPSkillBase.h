// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UPSkillData.h"
#include "UPSkillBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class UNIVERSITYPROJECT_API UUPSkillBase : public UActorComponent
{
	GENERATED_BODY()

public:
	UUPSkillBase();
	
protected:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Skill")
	void CustomActivate();
	void CustomActivate_Implementation();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Skill")
	void CustomDeActivate();
	void CustomDeActivate_Implementation();
	FORCEINLINE virtual float GetAttackDamage() { return SkillData->CustomDamageCalculation(GetOwner()); }
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Data")
	TObjectPtr<UUPSkillData> SkillData;
};
