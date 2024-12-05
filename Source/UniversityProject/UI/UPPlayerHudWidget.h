// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UPHudWidget.h"
#include "UPPlayerHudWidget.generated.h"

UENUM(BlueprintType)
enum class EPlayerExpressionType : uint8
{
	Common UMETA(DisplayName = "평범, 기쁨"),
	Exciting UMETA(DisplayName = "궁금"),
	Downcast UMETA(DisplayName = "시무룩"),
	Sad UMETA(DisplayName = "절망, 슬픔"),
	Angry UMETA(DisplayName = "화남"),
};

/**
 * 
 */
UCLASS()
class UNIVERSITYPROJECT_API UUPPlayerHudWidget : public UUPHudWidget
{
	GENERATED_BODY()
public:
	UUPPlayerHudWidget(const FObjectInitializer& ObjectInitializer);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "UI")
	float GetPlayerHpPercent() const;

	float GetPlayerHpPercent_Implementation() const;

	void SetFacialExpression(EPlayerExpressionType type);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TMap<EPlayerExpressionType, UTexture2D*> ImageMap;

	UPROPERTY(meta = (BindWidget))
	class UImage* PlayerFacialBg;
};
