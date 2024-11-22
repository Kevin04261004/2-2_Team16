// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/UPMonsterDeadAnimNotify.h"

#include "Character/Enemy/UPMonsterBase.h"

void UUPMonsterDeadAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                      const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (MeshComp)
	{
		AUPMonsterBase* Monster = Cast<AUPMonsterBase>(MeshComp->GetOwner());
		if (Monster)
		{
			Monster->DeadFadeOut();		
		}
	}
}
