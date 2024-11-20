// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/UPPettuPunchTrailAnimNotifyState.h"

#include "Character/Enemy/UPPettuCharacter.h"

void UUPPettuPunchTrailAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                    float InTotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, InTotalDuration);

	IUPPettuPunchTrailInterface* PettuCharacter = Cast<IUPPettuPunchTrailInterface>(MeshComp->GetOwner());
	if (PettuCharacter == nullptr)
	{
		return;
	}

	PettuCharacter->PunchTrailOn(PunchTrailType);
}

void UUPPettuPunchTrailAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

}

void UUPPettuPunchTrailAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	
	IUPPettuPunchTrailInterface* PettuCharacter = Cast<IUPPettuPunchTrailInterface>(MeshComp->GetOwner());
	if (PettuCharacter == nullptr)
	{
		return;
	}

	PettuCharacter->PunchTrailOff(PunchTrailType);
}
