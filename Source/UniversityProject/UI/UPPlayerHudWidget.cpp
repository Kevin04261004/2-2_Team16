// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UPPlayerHudWidget.h"
#include "Character/UPPlayerCharacter.h"
#include "Components/Image.h"

UUPPlayerHudWidget::UUPPlayerHudWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

float UUPPlayerHudWidget::GetPlayerHpPercent_Implementation() const
{
	if (PlayerCharacter)
	{
		return PlayerCharacter->GetStat()->GetCurrentHp() / PlayerCharacter->GetStat()->GetBaseStat().MaxHp;
	}
	return 0.0f;
}

void UUPPlayerHudWidget::SetFacialExpression(int32 Index)
{
	if (ImageArray.IsValidIndex(Index) && PlayerFacialBg)
	{
		UTexture2D* SelectedTexture = ImageArray[Index];
		if (SelectedTexture)
		{
			// 브러시 업데이트
			FSlateBrush TextureBrush;
			TextureBrush.SetResourceObject(SelectedTexture);
			PlayerFacialBg->SetBrush(TextureBrush);
		}
	}
}


