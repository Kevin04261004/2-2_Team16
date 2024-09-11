// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/UPComboEndAnimNotify.h"

#include "Interface/UPAnimationAttackCheckInterface.h"

void UUPComboEndAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                   const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	if (MeshComp)
	{
		IUPAnimationAttackCheckInterface* AttackPawn = Cast<IUPAnimationAttackCheckInterface>(MeshComp->GetOwner());
		if (AttackPawn)
		{
			AttackPawn->AttackComboEnd();
		}
	}
}
