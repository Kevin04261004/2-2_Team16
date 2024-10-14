// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UPComboInputableData.generated.h"

/**
 * 
 */
UCLASS()
class UNIVERSITYPROJECT_API UUPComboInputableData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = ComboData, Meta = (ToolTip = "애니메이션을 몇 프레임으로 나눌것인가?"))
	float DefaultFrameCount = 30.0f;
	UPROPERTY(EditAnywhere, Category = ComboData, Meta = (ToolTip = "첫번째 프레임부터 얼마까지 입력을 허용할 것인가?"))
	float InputAllowFrameCount = 18.0f;
	UPROPERTY(EditAnywhere, Category = ComboData, Meta = (ToolTip = "몇번째 프레임부터 InputAllowFrameCount프레임까지 다음 콤보 전환을 허용할 것인가?"))
	float AnimationChangeStartFrameCount = 12.0f;

	
};
