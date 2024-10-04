// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UPPlayerBaseState.h"
#include "Components/ActorComponent.h"
#include "UPStateManager.generated.h"

UENUM(BlueprintType)
enum class EPlayerStateType : uint8
{
	/* Movement */
	Idle UMETA(DisplayName = "Idle"),
	Walk UMETA(DisplayName = "걷기"),
	Run UMETA(DisplayName = "달리기"),
	Jump UMETA(DisplayName = "점프"),
	InAir UMETA(DisplayName = "공중"),
	Land  UMETA(DisplayName = "착지"),

	/* Attack */
	Attack UMETA(DisplayName = "연속 공격"),
	UpperCut UMETA(DisplayName = "어퍼컷 (달리기 공격)"),
	TakeDown UMETA(DisplayName = "넘어뜨리기 (상대가 기절상태일 때 공격)"),
	KnockOver UMETA(DisplayName = "내려찍기 (공중에서 공격)"),
	
	/* Skill */
	Dash UMETA(DisplayName = "대시"),
	
	/* Others... */
	Dead UMETA(DisplayName = "죽음"),
	Damaged UMETA(DisplayName = "데미지를 받음"),
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNIVERSITYPROJECT_API UUPStateManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	UUPStateManager();

	void UpdateState(EPlayerStateType NewState);
	FORCEINLINE EPlayerStateType GetCurrentState() const { return CurrentState; }
protected:
	EPlayerStateType CurrentState;
	TMap<EPlayerStateType, UUPPlayerBaseState*> StateMap;
};
