// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/UPDamagableActor.h"

#include "Components/UPCharacterStatComponent.h"
#include "GameData/UPCharacterStatData.h"
#include "UI/UPHpBarWidget.h"
#include "UI/UPWidgetComponent.h"

// Sets default values
AUPDamagableActor::AUPDamagableActor()
{
	Stat = CreateDefaultSubobject<UUPCharacterStatComponent>(TEXT("Stat"));
	static ConstructorHelpers::FObjectFinder<UUPCharacterStatData> StatDataRef(TEXT("/Game/UniversityProject/GameData/DA_PlayerCharacterStat.DA_PlayerCharacterStat"));
	Stat->SetBaseStat(StatDataRef.Object.Get()->Stat);

	HpBar = CreateDefaultSubobject<UUPWidgetComponent>(TEXT("Widget"));
	// HpBar->AttachToComponent();
	HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/UniversityProject/UI/WBP_HpBarWidget.WBP_HpBarWidget_C"));

	if (HpBarWidgetRef.Class != nullptr)
	{
		HpBar->SetWidgetClass(HpBarWidgetRef.Class);
		HpBar->SetWidgetSpace(EWidgetSpace::Screen);
		HpBar->SetDrawSize(FVector2D(150.0f, 15.0f));
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

float AUPDamagableActor::UPTakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	Stat->ApplyDamage(DamageAmount);
	return DamageAmount;
}

void AUPDamagableActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Stat->OnHpZero.AddUObject(this, &AUPDamagableActor::SetDead);
}

void AUPDamagableActor::SetDead()
{
	SetActorEnableCollision(false);
	Destroy();
}

void AUPDamagableActor::SetupCharacterWidget(class UUPUserWidget* InUserWidget)
{
	UUPHpBarWidget* HpBarWidget = Cast<UUPHpBarWidget>(InUserWidget);
	if (HpBarWidget != nullptr)
	{
		HpBarWidget->UpdateStat(Stat->GetBaseStat(), Stat->GetModifierStat());
		HpBarWidget->UpdateHpBar(Stat->GetCurrentHp());
		Stat->OnHpChanged.AddUObject(HpBarWidget, &UUPHpBarWidget::UpdateHpBar);
		Stat->OnStatChanged.AddUObject(HpBarWidget, &UUPHpBarWidget::UpdateStat);
	}
}
