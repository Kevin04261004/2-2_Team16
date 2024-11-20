// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/Notify/UPHitCheckAnimNotify.h"
#include "UPMonsterHitCheckAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class UNIVERSITYPROJECT_API UUPMonsterHitCheckAnimNotify : public UUPHitCheckAnimNotify
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notify")
	bool bAttached;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notify")
	FName SocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notify")
	float AttackRange;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notify")
	float Amount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notify")
	FVector CollisionLocation;
	
protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	
};
