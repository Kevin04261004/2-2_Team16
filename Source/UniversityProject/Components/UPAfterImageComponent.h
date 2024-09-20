// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UPAfterImageComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNIVERSITYPROJECT_API UUPAfterImageComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UUPAfterImageComponent();

	void Initialize(ACharacter& Character);
	void CreateAfterImage();
protected:
	TObjectPtr<ACharacter> OwningCharacter;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category =Init, Meta = (AllowPrivateAccess = "true", Tooltip = "캐릭터 잔상 이펙트"))
	TObjectPtr<UClass> AfterImageClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Init, Meta = (AllowPrivateAccess = "true", Tooltip = "캐릭터로부터 얼마나 떨어져서 생성되는지"))
	FVector PositionOffset;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Init, Meta = (AllowPrivateAccess = "true", Tooltip = "캐릭터로부터 얼마나 회전해서 생성되는지"))
	FRotator RotationOffset;

};
