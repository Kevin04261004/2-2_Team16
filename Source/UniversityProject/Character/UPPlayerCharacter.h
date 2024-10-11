// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/UPCharacterBase.h"
#include "Components/UPInputHandlerComponent.h"
#include "Interface/UPAfterImageableInterface.h"
#include "Interface/UPAnimationAttackCheckInterface.h"
#include "Interface/UPCharacterGoForwardInterface.h"
#include "Interface/UPCharacterHUDInterface.h"
#include "Player/UPPlayerController.h"
#include "State/UPStateManager.h"
#include "UPPlayerCharacter.generated.h"

UENUM(BlueprintType)
enum class EPlayerSkillType : uint8
{
	None UMETA(DisplayName = "Hidden"),
	BaseAttack01 UMETA(DisplayName="공격 1타"),
	BaseAttack02 UMETA(DisplayName="공격 2타"),
	BaseAttack03 UMETA(DisplayName="공격 3타"),
	UpperCut UMETA(DisplayName="어퍼컷"),
	TakeDown UMETA(DisplayName="내려 찍기"),
	KnockOver UMETA(DisplayName="넘어뜨리기"),
	Dash UMETA(DisplayName="대시"),
};

class UUPStateManager;

/**
 * 
 */
UCLASS()
class UNIVERSITYPROJECT_API AUPPlayerCharacter : public AUPCharacterBase, public IUPAnimationAttackCheckInterface, public IUPCharacterGoForwardInterface, public IUPAfterImageableInterface, public IUPCharacterHUDInterface
{
	GENERATED_BODY()

public:
	AUPPlayerCharacter(const FObjectInitializer& ObjectInitializer);
	virtual void PostInitializeComponents() override;

protected:
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	virtual void SetDead() override;

protected:
	TObjectPtr<AUPPlayerController> PlayerController;
	
/* Input Section */
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = InputHandler, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UUPInputHandlerComponent> InputHandler;

public:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

/* Camera Section */
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class USpringArmComponent> CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UCameraComponent> FollowCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UUPCameraComponent> CameraComponent;
	
/* After Image */
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=AfterImage, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UUPAfterImageComponent> AfterImageComponent;
	
	virtual void CreateAfterImage() override;

/* Check Hit Collision */
protected:
	virtual void AttackHitCheck() override;

/* Physics Section */
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= Init, Meta = (AllowPrivateAccess = "true", Tooltip = "공격 시 얼마나 앞으로 이동하는가"))
	float GoForwardDistance;

	virtual void GoForward() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AfterImage, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UPhysicsControlComponent> PhysicsControlComponent;

	virtual bool CanJumpInternal_Implementation() const override;
/* AI Section */
protected:
	TObjectPtr<class UAIPerceptionStimuliSourceComponent> StimuliSource;
	void SetupStimuliSource();

/* State Section */
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSM, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UUPStateManager> StateManager;

public:
	FORCEINLINE UUPStateManager* GetStateManager() { return StateManager; }

/* Skill Section */
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UUPSkillManagerComponent> SkillManager;

public:
	FORCEINLINE UUPSkillManagerComponent* GetSkillManager() { return SkillManager; }

/* UI Section */
protected:
	virtual void SetupHUDWidget(UUPHudWidget* InHUDWidget) override;
};
