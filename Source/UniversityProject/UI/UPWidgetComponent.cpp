// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UPWidgetComponent.h"

#include "Interface/UPUserWidget.h"

void UUPWidgetComponent::InitWidget()
{
	Super::InitWidget();
	
	UUPUserWidget* ABUserWidget = Cast<UUPUserWidget>(GetWidget());
	if (ABUserWidget)
	{
		ABUserWidget->SetOwningActor(GetOwner());
	}
}
