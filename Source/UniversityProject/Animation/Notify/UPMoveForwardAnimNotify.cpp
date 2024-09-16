// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/UPMoveForwardAnimNotify.h"

#include "Interface/UPCharacterGoForwardInterface.h"

void UUPMoveForwardAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                      const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		IUPCharacterGoForwardInterface* AttackPawn = Cast<IUPCharacterGoForwardInterface>(MeshComp->GetOwner());
		if (AttackPawn)
		{
			AttackPawn->GoForward();
		}
	}
}
