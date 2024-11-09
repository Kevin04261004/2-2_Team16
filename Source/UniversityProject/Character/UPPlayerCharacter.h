// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/UPCharacterBase.h"
#include "Components/UPInputHandlerComponent.h"
#include "Enemy/UPPettuCharacter.h"
#include "Interface/UPAfterImageableInterface.h"
#include "Interface/UPAnimationAttackCheckInterface.h"
#include "Interface/UPCharacterHUDInterface.h"
#include "Interface/UPResetAttackedActorList.h"
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
	Damaged UMETA(DisplayName="피격"),
};

class UUPStateManager;

/**
 * 
 */
UCLASS()
class UNIVERSITYPROJECT_API AUPPlayerCharacter : public AUPCharacterBase,
public IUPAnimationAttackCheckInterface, public IUPAfterImageableInterface,
public IUPCharacterHUDInterface, public IUPResetAttackedActorList
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
	FORCEINLINE UUPInputHandlerComponent* GetInputHandler() { return InputHandler; }
public:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	FVector GetLastInputVector() const;
/* Camera Section */
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class USpringArmComponent> CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UCameraComponent> FollowCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UUPCameraComponent> CameraComponent;

public:
	FORCEINLINE UUPCameraComponent* GetCameraComponent() const { return CameraComponent.Get(); }
	
/* After Image */
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=AfterImage, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UUPAfterImageComponent> AfterImageComponent;
	
	virtual void CreateAfterImage() override;

/* Check Hit Collision */
protected:
	virtual float UPTakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void AttackHitCheck() override;

protected:
	virtual void ResetAttackedActorList() override;
	
/* Physics Section */
protected:
	virtual bool CanJumpInternal_Implementation() const override;
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AfterImage, Meta = (AllowPrivateAccess = true))
	TObjectPtr<class UPhysicsControlComponent> PhysicsControlComponent;
	
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

/* Look At Section */
private:
	UPROPERTY()
	float LookAtAlpha = 0.0f;
	float TargetAlpha = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = LookAt, Meta = (AllowPrivateAccess = true))
	float LookAtFOV = 50.0f;
	UPROPERTY()
	FVector LookAtLocation = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = LookAt, Meta = (AllowPrivateAccess = true))
	FName LookAtLocationSocketName;
	
	void SetLookAtAlpha(float DeltaTime);
	
	UPROPERTY()
	TObjectPtr<AUPPettuCharacter> PettuCharacter;
public:
	UFUNCTION(BlueprintCallable)
	float GetLookAtAlpha() const { return LookAtAlpha; }
	UFUNCTION(BlueprintCallable)
	float GetLookAtFOV() const { return LookAtFOV; }
	UFUNCTION(BlueprintCallable)
	FVector GetLookAtLocation() const { return LookAtLocation; }

	void SetPettuCharacter(AUPPettuCharacter* InPettuCharacter) { PettuCharacter = InPettuCharacter; }

	UFUNCTION(BlueprintCallable)
	AUPPettuCharacter* GetPettuCharacter() const { return PettuCharacter.Get(); }
};
