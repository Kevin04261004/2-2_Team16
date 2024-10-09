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


	virtual void Initialize(class UUPCharacterMovementComponent* InCharacterMovementComponent, class AUPPlayerCharacter* InPlayerCharacter);
	float GetSkillAttackDamage();

	// 외부에서(캐릭터에서) 스킬을 실행!
	bool TryActivateSkill(AActor* TargetOrNull);

	// 외부에서(캐릭터에서) 스킬을 중지!
	bool TryStopSkill();
	
	// SkillData
	FORCEINLINE UUPSkillData* GetSkillData() { return SkillData; }

	bool CanUseSkill();
protected:
	// 게임 시작될 때 한번 호출이 됩니다.
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
protected:
	class UUPCharacterMovementComponent* CharacterMovementComponent;
	class AUPPlayerCharacter* PlayerCharacter;
	
/* Customize Section */
protected:
	// 스킬에 대한 모든 정보가 들어가있는 데이터 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Data")
	TObjectPtr<UUPSkillData> SkillData;

	// 기획자가 블루프린트에서 커스터마이징 가능한 함수
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Skill", meta = (Tooltip = "스킬이 발생되었을 때 구현"))
	void CustomActivate(AActor* TargetOrNull);

	UFUNCTION(BlueprintType, BlueprintCallable, Category="Skill")
	// 기획자가 커스터마이징을 하지 않으면 아래 함수(<커스텀 함수명>_Implementation)가 호출이 되어요!!
	virtual void CustomActivate_Implementation(AActor* TargetOrNull);

	// 기획자가 블루프린트에서 커스터마이징 가능한 함수
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Skill", meta = (Tooltip = "스킬이 스탑되었을 때 구현"))
	void CustomStop();

	UFUNCTION(BlueprintType, BlueprintCallable, Category="Skill")
	// 기획자가 커스터마이징을 하지 않으면 아래 함수(<커스텀 함수명>_Implementation)가 호출이 되어요!!
	virtual void CustomStop_Implementation();
	
	// 기획자가 블루프린트에서 커스터마이징 가능한 함수
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Skill", meta = (Tooltip = "스킬이 종료되었을 때 구현"))
	void CustomDeActivate(AActor* TargetOrNull);
	
	UFUNCTION(BlueprintType, BlueprintCallable, Category="Skill")
	// 기획자가 커스터마이징을 하지 않으면 아래 함수(<커스텀 함수명>_Implementation)가 호출이 되어요!!
	virtual void CustomDeActivate_Implementation(AActor* TargetOrNull);

	UFUNCTION(BlueprintType, BlueprintCallable, Category="Skill")
	FORCEINLINE FVector GetOwnerLocation() const { return GetOwner()->GetActorLocation(); }
/* Util */
protected:
	float GetSkillDuration() const;
	
/* Current Skill Data Section */
protected:
	float CurCoolTime;
	bool bIsSkillActive;
	FTimerHandle SkillDurationHandle;
	FTimerDelegate DeActivateSkillTimerDelegate;

protected:
	UFUNCTION(BlueprintType, BlueprintCallable, Category="Skill")
	void SetOwnerMovementMode(EMovementMode mode);
};
