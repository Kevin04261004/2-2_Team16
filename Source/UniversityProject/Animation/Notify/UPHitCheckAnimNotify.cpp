// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/UPHitCheckAnimNotify.h"

#include "Interface/UPAnimationAttackCheckInterface.h"

void UUPHitCheckAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                   const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		IUPAnimationAttackCheckInterface* AttackPawn = Cast<IUPAnimationAttackCheckInterface>(MeshComp->GetOwner());
		if (AttackPawn)
		{
			AttackPawn->AttackHitCheck();
		}
	}
}
