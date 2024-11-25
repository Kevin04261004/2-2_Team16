// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/UPMonsterBase.h"

#include "BrainComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Character/Weapon/UPPettuWeapon.h"
#include "Character/Weapon/UPWeaponBase.h"
#include "Skill/UPPettuSkillData.h"
#include "Skill/UPSkillBase.h"


AUPMonsterBase::AUPMonsterBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UUPCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	InitSkillMap();
	
}

void AUPMonsterBase::BeginPlay()
{
	Super::BeginPlay();
	CreateDefaultObjectSkill();
	MovementComponent->SetMovementMode(MOVE_Walking);
	MonsterAIController = Cast<AUPPettuAIController>(GetController());
}

void AUPMonsterBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AUPMonsterBase::InitSkillMap()
{
	const UEnum* EnumPtr = StaticEnum<EPettuSkillType>();
	if (!EnumPtr)
	{
		return;
	}
	
	for (int32 i = 0; i < EnumPtr->GetMaxEnumValue(); ++i)  // GetMaxEnumValue()를 사용
	{
		EPettuSkillType EnumValue = static_cast<EPettuSkillType>(EnumPtr->GetValueByIndex(i));  // GetValueByIndex() 사용
		SkillMapInitializer.Add(EnumValue, nullptr);
	}
}

void AUPMonsterBase::CreateDefaultObjectSkill()
{
	for (auto skillMapTuple : SkillMapInitializer)
	{
		if (skillMapTuple.Value == nullptr)
		{
			continue;
		}
		
		if (UUPSkillBase* NewSkillComponent = NewObject<UUPSkillBase>(this, skillMapTuple.Value))
		{
			// 컴포넌트를 월드에 등록합니다.
			NewSkillComponent->RegisterComponent();

			// SkillMap에 새로 생성된 컴포넌트를 추가합니다.
			EPettuSkillType Type = skillMapTuple.Key;
			SkillMap.Add(Type, NewSkillComponent);

			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Skill Map Created"));
		}
	}
}

void AUPMonsterBase::SkillAttack(EPettuSkillType SkillType)
{
	UUPSkillBase** Skill = SkillMap.Find(SkillType);
	CurrentSkillData = Cast<UUPPettuSkillData>((*Skill)->GetSkillData());
	(*Skill)->TryActivateSkill(nullptr);
	CurAttackDamage = (*Skill)->GetSkillAttackDamage();
}

void AUPMonsterBase::SetDead()
{
	Super::SetDead();
	//StageManager->EvaluateCondition(DiedCondition);
	//SetActorEnableCollision(false);
	//bIsDead = true;
	MovementComponent->DisableMovement();
	if (MonsterAIController)
	{
		if (MonsterAIController->BrainComponent)
		{
			MonsterAIController->BrainComponent->StopLogic(TEXT("Dead"));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("BrainComponent is nullptr"));
		}
		UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(MonsterAIController->BrainComponent);
		if (BTComp)
		{
			BTComp->StopTree(EBTStopMode::Forced);
		}
		MonsterAIController->StopMovement();
	}
	//Weapon->Destroy();
}

void AUPMonsterBase::DeadAnimEnd(UAnimMontage* Montage, bool bInterrupted)
{
	Super::DeadAnimEnd(Montage, bInterrupted);
	
}

void AUPMonsterBase::PlayDeadAnimation()
{
	// Super::PlayDeadAnimation();
}

void AUPMonsterBase::DeadFadeOut()
{
	// 1. 평상시 사용하던 머티리얼을 Dissolve 머티리얼로 교체
	for (int32 i = 0; i < GetMesh()->GetNumMaterials(); i++)
	{
		UMaterialInterface* OriginalMaterial = GetMesh()->GetMaterial(i);

		// Dissolve 머티리얼로 교체
		UMaterialInstanceDynamic* DissolveMaterial = UMaterialInstanceDynamic::Create(DissolveMaterialTemplate, this);
		if (DissolveMaterial)
		{
			// 초기 Dissolve 값을 설정 (1.0 -> 완전히 불투명)
			DissolveMaterial->SetScalarParameterValue(TEXT("Dissolve"), 1.0f);
			GetMesh()->SetMaterial(i, DissolveMaterial);

			// 캐싱하여 나중에 업데이트에 사용
			CachedDissolveMaterials.Add(DissolveMaterial);
		}
	}

	// Dissolve 효과 시작
	StartDissolve();
}

void AUPMonsterBase::StartDissolve()
{
	const float DissolveDuration = 2.0f; // Dissolve 효과의 총 시간
	const float DissolveInterval = 0.05f; // 업데이트 간격
	CurrentDissolveValue = 0.0f; // 초기 Dissolve 값 설정 (완전히 보이는 상태)

	// Dissolve 업데이트 타이머 시작
	GetWorldTimerManager().SetTimer(DissolveTimerHandle, this, &AUPMonsterBase::UpdateDissolve, DissolveInterval, true);

	// Dissolve 종료 타이머 설정
	GetWorldTimerManager().SetTimer(DissolveEndHandle, this, &AUPMonsterBase::FinishDissolve, DissolveDuration, false);
}

void AUPMonsterBase::UpdateDissolve()
{
	// Dissolve 값을 점진적으로 증가
	CurrentDissolveValue += 0.05f;

	if (CurrentDissolveValue > 2.0f)
	{
		CurrentDissolveValue = 2.0f;
	}

	// 모든 캐싱된 머티리얼에 Dissolve 값 업데이트
	for (UMaterialInstanceDynamic* DissolveMaterial : CachedDissolveMaterials)
	{
		if (DissolveMaterial)
		{
			DissolveMaterial->SetScalarParameterValue(TEXT("Dissolve"), CurrentDissolveValue);
		}
	}
}

void AUPMonsterBase::FinishDissolve()
{
	// 타이머 정지
	GetWorldTimerManager().ClearTimer(DissolveTimerHandle);

	// 액터 제거
	Destroy();
}

void AUPMonsterBase::SetWeaponCollision(bool bIsCollision)
{
	if (Weapon)
	{
		AUPPettuWeapon* MonsterWeapon = Cast<AUPPettuWeapon>(Weapon);
		MonsterWeapon->SetCollision(bIsCollision);
	}
}
