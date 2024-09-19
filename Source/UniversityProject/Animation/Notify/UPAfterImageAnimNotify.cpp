// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/UPAfterImageAnimNotify.h"

#include "Interface/UPAfterImageableInterface.h"

void UUPAfterImageAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                     const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	
	if (MeshComp)
	{
		IUPAfterImageableInterface* AttackPawn = Cast<IUPAfterImageableInterface>(MeshComp->GetOwner());
		if (AttackPawn)
		{
			AttackPawn->CreateAfterImage();
		}
	}
}
