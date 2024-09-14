#include "UPPostProcessManager.h"

#include "EngineUtils.h"

void UUPPostProcessManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimerForNextTick([this]()
		{
			FindPostProcessVolume();
		});
	}
}

void UUPPostProcessManager::FindPostProcessVolume()
{
	UWorld* World = GetWorld();
	if (World)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("World found"));
		for (TActorIterator<APostProcessVolume> It(World); It; ++It)
		{            
			PostProcessVolume = *It;
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("PostProcessingVolume found"));
			break;
		}

		if (PostProcessVolume != nullptr)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("No PostProcessingVolume found"));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("No World found"));
	}
}

void UUPPostProcessManager::TogglePostProcessMaterial(EPostProcessMaterialType MaterialType, bool bEnable, float ActiveTime)
{
	if (PostProcessVolume == nullptr)
	{
		return;
	}
	if (MaterialType >= 0 && MaterialType < PostProcessVolume->Settings.WeightedBlendables.Array.Num())
	{
		FWeightedBlendable& Blendable = PostProcessVolume->Settings.WeightedBlendables.Array[MaterialType];

		if (bEnable)
		{
			Blendable.Weight = 1.0f;
		}
		else
		{
			Blendable.Weight = 0.0f;
		}

		if (ActiveTime > 0.0f)
		{
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, MaterialType, bEnable]()
			{
				TogglePostProcessMaterial(MaterialType, !bEnable);
			}, ActiveTime, false);
		}
	}
}