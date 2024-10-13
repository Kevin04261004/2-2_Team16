// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/UPGoUpAnimNotify.h"

#include "Interface/UPTakeGoUpInterface.h"

void UUPGoUpAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                              const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

		
	if (MeshComp)
	{
		IUPTakeGoUpInterface* AttackPawn = Cast<IUPTakeGoUpInterface>(MeshComp->GetOwner());
		if (AttackPawn)
		{
			AttackPawn->TryGoUp(GoUpAmount);
		}
	}
}
