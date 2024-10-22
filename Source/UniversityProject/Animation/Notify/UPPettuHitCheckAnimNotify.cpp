// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/UPPettuHitCheckAnimNotify.h"

#include "Character/Enemy/UPPettuCharacter.h"

void UUPPettuHitCheckAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                        const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		AUPPettuCharacter* AttackPawn = Cast<AUPPettuCharacter>(MeshComp->GetOwner());
		if (AttackPawn)
		{
			USkeletalMeshComponent* Mesh = AttackPawn->GetMesh();
			AttackPawn->AttackHitCheck(bAttached, SocketName, Mesh, AttackRange, Amount, CollisionLocation);
			
		}
	}
}
