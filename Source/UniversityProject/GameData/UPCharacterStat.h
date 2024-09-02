#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UPCharacterStat.generated.h"

USTRUCT(BlueprintType)
struct FUPCharacterStat : public FTableRowBase
{
	GENERATED_BODY()

// Stat Section <Only float Value>
public:
	FUPCharacterStat() : MaxHp(0.0f), AttackDamage(0.0f), AttackRange(0.0f), AttackSpeed(0.0f), MovementSpeed(0) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float AttackDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float AttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MovementSpeed;

	FUPCharacterStat operator+(const FUPCharacterStat& Other) const
	{
		const float* const ThisPtr = reinterpret_cast<const float* const>(this);
		const float* const OtherPtr = reinterpret_cast<const float* const>(&Other);
		
		FUPCharacterStat Result;
		float* ResultPtr = reinterpret_cast<float*>(&Result);
		int32 statNum = sizeof(FUPCharacterStat) / sizeof(float);
		for (int32 i = 0; i < statNum; i++)
		{
			ResultPtr[i] = ThisPtr[i] + OtherPtr[i];
		}
		return Result;
	}
};