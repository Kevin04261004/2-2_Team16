// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/UP3DSoundAnimNotify.h"

#include "Audio/UPAudioManager.h"

void UUP3DSoundAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
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
		AudioManager->PlaySoundAtLocation(Audio, TargetLocation);
	}
}
