// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/UPResetAttackedActorsAnimNotify.h"

#include "Interface/UPResetAttackedActorList.h"

void UUPResetAttackedActorsAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                              const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	if (MeshComp)
	{
		IUPResetAttackedActorList* AttackPawn = Cast<IUPResetAttackedActorList>(MeshComp->GetOwner());
		if (AttackPawn)
		{
			AttackPawn->ResetAttackedActorList();
		}
	}
}
