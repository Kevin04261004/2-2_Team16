// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Audio/UPAudioManager.h"
#include "UP3DSoundEnumAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class UNIVERSITYPROJECT_API UUP3DSoundEnumAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio, meta = (AllowPrivateAccess = "true"))
	ESFXAudioType SFXAudio;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
};
