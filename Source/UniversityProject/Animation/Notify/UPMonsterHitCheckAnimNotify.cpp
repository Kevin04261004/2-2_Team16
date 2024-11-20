// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/UPMonsterHitCheckAnimNotify.h"

#include "Character/Enemy/UPEnemyMonster.h"

void UUPMonsterHitCheckAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                          const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (MeshComp)
	{
		AUPEnemyMonster* AttackPawn = Cast<AUPEnemyMonster>(MeshComp->GetOwner());
		if (AttackPawn)
		{
			USkeletalMeshComponent* Mesh = AttackPawn->GetMesh();
			AttackPawn->AttackHitCheck(bAttached, SocketName, Mesh, AttackRange, Amount, CollisionLocation);
		}
	}
}
