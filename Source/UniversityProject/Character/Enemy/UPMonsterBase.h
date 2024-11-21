// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/UPCharacterBase.h"
#include "AI/UPPettuAIController.h"
#include "Skill/UPPettuSkillData.h"
#include "Skill/UPSkillBase.h"
#include "UPMonsterBase.generated.h"

UENUM(BlueprintType)
enum class EPettuSkillType : uint8
{
	SmashAttack UMETA(DisplayName = "휘두르기1"),
	SmashAttack2 UMETA(DisplayName = "휘두르기2"),
	TakeTurnGroundAttack UMETA(DisplayName = "두 손 번갈아 내려찍기"),
	JumpGroundAttack UMETA(DisplayName = "점프 내려찍기"),
	TwoHandGroundAttack UMETA(DisplayName = "두 손 내려찍기"),
};

UCLASS()
class UNIVERSITYPROJECT_API AUPMonsterBase : public AUPCharacterBase
{
	GENERATED_BODY()
public:
	AUPMonsterBase(const FObjectInitializer& ObjectInitializer);
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

/* AI Section */
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = AI, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBehaviorTree> BTree;
	
	TObjectPtr<class UBehaviorTree> GetBehaviorTree() const { return BTree; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsRotating = false;

/* Skill Section */
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Init, Meta = (AllowPrivateAccess = true))
	TMap<EPettuSkillType, TSubclassOf<UUPSkillBase>> SkillMapInitializer;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Init, Meta = (AllowPrivateAccess = true))
	TMap<EPettuSkillType, UUPSkillBase*> SkillMap;

	UPROPERTY()
	TObjectPtr<UUPPettuSkillData> CurrentSkillData;
	
	void InitSkillMap();
	void CreateDefaultObjectSkill();

	UPROPERTY()
	EPettuSkillType CurrentSkillType;
	
public:
	virtual void SkillAttack(EPettuSkillType SkillType);
};
