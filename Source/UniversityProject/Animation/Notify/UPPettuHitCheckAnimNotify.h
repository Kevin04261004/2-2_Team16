// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UPPettuHitCheckAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class UNIVERSITYPROJECT_API UUPPettuHitCheckAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notify")
	bool bAttached;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notify")
	FName SocketName;
	
protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	
};
