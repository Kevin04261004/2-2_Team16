// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/UP3DSoundEnumAnimNotify.h"

void UUP3DSoundEnumAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	if (MeshComp != nullptr)
	{
		FVector TargetLocation = MeshComp->GetOwner()->GetActorLocation();

		UGameInstance* GameInstance = MeshComp->GetWorld()->GetGameInstance();
		if (GameInstance == nullptr)
		{
			return;
		}
		
		UUPAudioManager* AudioManager = GameInstance->GetSubsystem<UUPAudioManager>();
		
		if (AudioManager == nullptr)
		{
			return;
		}
		AudioManager->PlaySoundAtLocation(SFXAudio, TargetLocation);
	}
}
