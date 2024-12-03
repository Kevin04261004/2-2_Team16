// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UPStateManager.generated.h"


UENUM(BlueprintType)
enum class EPlayerStateType : uint8
{
	None UMETA(DisplayName = "Hidden"),
	/* Movement */
	Idle UMETA(DisplayName = "Idle"),
	Sprint UMETA(DisplayName = "달리기"),
	Jump UMETA(DisplayName = "점프"),
	InAir UMETA(DisplayName = "공중"),
	Land  UMETA(DisplayName = "착지"),

	/* Attack */
	BaseAttack01 UMETA(DisplayName = "기본 공격01"),
	BaseAttack02 UMETA(DisplayName = "기본 공격02"),
	BaseAttack03 UMETA(DisplayName = "기본 공격03"),
	UpperCut UMETA(DisplayName = "어퍼컷 (달리기 공격)"),
	TakeDown UMETA(DisplayName = "내려찍기 (공중에서 공격)"),
	KnockOver UMETA(DisplayName = "넘어뜨리기 (상대가 기절상태일 때 공격)"), //
	
	/* Skill */
	Dash UMETA(DisplayName = "대시"),
	
	/* Others... */
	Dead UMETA(DisplayName = "죽음"), //
	Damaged UMETA(DisplayName = "데미지를 받음"),
	CantMove UMETA(DisplayName = "움직일 수 없음")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNIVERSITYPROJECT_API UUPStateManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	UUPStateManager();
	
	void Initialize(class UUPInputHandlerComponent* InInputHandler);
	void InitializeStates(EPlayerStateType InitState);
	void InitializeStateMap();

	void UpdateState();
	void ChangeState(EPlayerStateType NextState);
	class UUPPlayerBaseState* GetState(EPlayerStateType InState);

	FORCEINLINE EPlayerStateType GetCurrentState() const { return CurrentStateType; }
protected:
	UPROPERTY()
	EPlayerStateType CurrentStateType;

	UPROPERTY()
	TMap<EPlayerStateType, class UUPPlayerBaseState*> StateMap;

protected:
	UPROPERTY()
	TObjectPtr<class AUPPlayerCharacter> OwningCharacter;
	UPROPERTY()
	TObjectPtr<class UUPInputHandlerComponent> InputHandler;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Init, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UUPComboInputableData> ComboInputableData_Combo1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Init, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UUPComboInputableData> ComboInputableData_Combo2;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Init, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UUPComboInputableData> ComboInputableData_Combo3;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Init, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> TakeDownToIdleMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Init, Meta = (AllowPrivateAccess = true))
	float JumpToTakeDownMinVelocity = 1200.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Init, Meta = (AllowPrivateAccess = true))
	float TakeDownVelocity = 5000.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Init, Meta = (AllowPrivateAccess = true))
	float TakeDownAttackRadius = 125.f;
};
