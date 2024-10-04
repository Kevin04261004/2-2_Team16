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
enum class EPlayerSkillType : uint8
{
	None UMETA(DisplayName = "Hidden"),
	Attack01 UMETA(DisplayName="공격 1타"),
	Attack02 UMETA(DisplayName="공격 2타"),
	Attack03 UMETA(DisplayName="공격 3타"),
	UpperCut UMETA(DisplayName="어퍼컷"),
	TakeDown UMETA(DisplayName="내려 찍기"),
	KnockOver UMETA(DisplayName="넘어뜨리기"),
	Throw UMETA(DisplayName="투척"),
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
	TObjectPtr<class UInputAction> RapidAttackAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Init, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> HeavyAttackAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Init, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ESkillAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Init, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> RSkillAction;
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
	void RapidAttackInputAction(const FInputActionValue& Value);
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
	virtual void AttackHitCheck() override;

/* Physics Section */
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= Init, Meta = (AllowPrivateAccess = "true", Tooltip = "공격 시 얼마나 앞으로 이동하는가"))
	float GoForwardDistance;
	virtual void GoForward() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AfterImage, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UPhysicsControlComponent> PhysicsControlComponent;
	
/* AI Section */
protected:
	TObjectPtr<class UAIPerceptionStimuliSourceComponent> StimuliSource;
	void SetupStimuliSource();

/* State Section */
protected:
	

};
