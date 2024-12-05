// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UPHudWidget.h"
#include "UPPettuHudWidget.h"
#include "UPPlayerHudWidget.h"


UUPHudWidget::UUPHudWidget(const FObjectInitializer& ObjectInitializer)
{
}

void UUPHudWidget::SetPlayerHudVisible(bool bVisible)
{
	if (PlayerHudWidget)
	{
		PlayerHudWidget->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void UUPHudWidget::SetPettuHudVisible(bool bVisible)
{
	if (PettuHudWidget)
	{
		PettuHudWidget->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void UUPHudWidget::SetExpression(EPlayerExpressionType expressionType)
{
	if (PlayerHudWidget)
	{
		PlayerHudWidget->SetFacialExpression(expressionType);
	}
}





