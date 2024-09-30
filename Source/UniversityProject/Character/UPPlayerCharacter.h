// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/UPCharacterBase.h"
#include "InputActionValue.h"
#include "Components/UPCameraComponent.h"
#include "Interface/UPAfterImageableInterface.h"
#include "Interface/UPAnimationAttackCheckInterface.h"
#include "Interface/UPCharacterGoForwardInterface.h"
#include "Player/UPPlayerController.h"
#include "Skill/UPSkillBase.h"
#include "UPPlayerCharacter.generated.h"

UENUM(BlueprintType)
enum class ESkillType : uint8
{
	RapidAttack01 UMETA(DisplayName="약 공격 1타"),
	RapidAttack02 UMETA(DisplayName="약 공격 2타"),
	RapidAttack03 UMETA(DisplayName="약 공격 3타"),
	RapidAttackFinal UMETA(DisplayName="약 공격 마무리"),
	HeavyAttack01 UMETA(DisplayName="강 공격 1타"),
	HeavyAttack02 UMETA(DisplayName="강 공격 2타"),
	HeavyAttackFinal01 UMETA(DisplayName="강 공격 마무리"),
	HeavyAttackFinal02 UMETA(DisplayName="강 공격 마무리"),
	HeavyAttackFinal03 UMETA(DisplayName="강  공격 마무리"),
	RapidTakeDownAttack UMETA(DisplayName="약 내려찍기"),
	HeavyTakeDownAttack UMETA(DisplayName="강 내려찍기"),
	RapidRunningAttack UMETA(DisplayName="달리기 약 공격"),
	HeavyRunningAttack UMETA(DisplayName="달리기 강 공격"),
};

/**
 * 
 */
UCLASS()
class UNIVERSITYPROJECT_API AUPPlayerCharacter : public AUPCharacterBase, public IUPAnimationAttackCheckInterface, public IUPCharacterGoForwardInterface, public IUPAfterImageableInterface
{
	GENERATED_BODY()

public:
	AUPPlayerCharacter(const FObjectInitializer& ObjectInitializer);
	virtual void PostInitializeComponents() override;

protected:
	virtual void BeginPlay() override;
	virtual void SetDead() override;
public:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	TObjectPtr<AUPPlayerController> PlayerController;
	
// Input Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Init, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> IMC_BackView;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Init, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Init, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AttackAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Init, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> DashAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Init, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Init, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Init, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> CameraZoomAction;
	
	void MoveInputAction(const FInputActionValue& Value);
	void LookInputAction(const FInputActionValue& Value);
	void AttackInputAction(const FInputActionValue& Value);
	void DashInputAction(const FInputActionValue& Value);
	void JumpInputAction(const FInputActionValue& Value);
	void WalkInputAction(const FInputActionValue& Value);
	void ZoomCameraInputAction(const FInputActionValue& Value);
		
/* Camera Section */
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, Meta = (AllowPrivateAccess = true))
	TObjectPtr<USpringArmComponent> CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UCameraComponent> FollowCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UUPCameraComponent> CameraComponent;
/* dash Section */
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=AfterImage, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UUPDashComponent> DashComponent;
	
/* After Image */
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=AfterImage, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UUPAfterImageComponent> AfterImageComponent;
	
	virtual void CreateAfterImage() override;
/* ComboAttack Section */
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ComboAttack, Meta = (AllowPrivateAccess = "true", Tooltip = "콤보 공격 컴포넌트"))
	TObjectPtr<class UUPComboAttackComponent> ComboAttack;
	
	virtual void AttackHitCheck() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= Init, Meta = (AllowPrivateAccess = "true", Tooltip = "공격 시 얼마나 앞으로 이동하는가"))
	float GoForwardDistance;
/* Physics Section */
protected:
	virtual void GoForward() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AfterImage, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UPhysicsControlComponent> PhysicsControlComponent;
/* AI Section */
protected:
	TObjectPtr<class UAIPerceptionStimuliSourceComponent> StimuliSource;
	void SetupStimuliSource();

/* Auto Targeting */
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AutoTargeting, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UAutoTargetingComponent> AutoTargetingComponent;

/* Skill Section */
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Init, Meta = (AllowPrivateAccess = true))
	TMap<ESkillType, TSubclassOf<UUPSkillBase>> SkillMapInitializer;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Init, Meta = (AllowPrivateAccess = true))
	TMap<ESkillType, UUPSkillBase*> SkillMap;
	void InitSkillMap();
	void CreateDefaultObjectSkill();
};
