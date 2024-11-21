// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/UPMonsterBase.h"

#include "BrainComponent.h"
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
	MovementComponent->DisableMovement();
	if (MonsterAIController)
	{
		if (MonsterAIController->BrainComponent)
		{
			MonsterAIController->BrainComponent->StopLogic(TEXT("Dead"));
		}
		MonsterAIController->StopMovement();
	}
}

void AUPMonsterBase::DeadAnimEnd(UAnimMontage* Montage, bool bInterrupted)
{
	Super::DeadAnimEnd(Montage, bInterrupted);

	//GetMesh()->Stop();
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	//if (Montage)
	//{
	//	float LastFrameTime = Montage->GetPlayLength();
	//	GetMesh()->SetPosition(LastFrameTime); // 애니메이션 마지막 프레임으로 이동
	//	GetMesh()->SetPlayRate(0.0f);          // 재생 정지
	//}
	if (DeathAnim)
	{
		GetMesh()->PlayAnimation(DeathAnim, false); // 애니메이션 재생 (루프 없음)
		GetMesh()->SetPosition(DeathAnim->GetPlayLength()); // 마지막 프레임으로 이동
	}
	
	// 다이나믹 머티리얼 캐싱 (한 번만 생성)
	CachedDynamicMaterials.Empty(); // 기존 캐싱 데이터 초기화
	for (int32 i = 0; i < GetMesh()->GetNumMaterials(); i++)
	{
		UMaterialInstanceDynamic* DynamicMaterial = GetMesh()->CreateAndSetMaterialInstanceDynamic(i);
		if (DynamicMaterial)
		{
			CachedDynamicMaterials.Add(DynamicMaterial);
			DynamicMaterial->SetScalarParameterValue(FName("Opacity"), 1.0f); // 초기값 설정
		}
	}

	StartFadeOut();
}

void AUPMonsterBase::StartFadeOut()
{
	// 1초마다 업데이트되는 타이머로 투명도 변경
	const float FadeDuration = 2.0f; // 사라지는 데 걸리는 시간
	const float FadeInterval = 0.1f; // 업데이트 간격
	CurrentFadeAlpha = 1.0f; // 초기 투명도 설정

	// 타이머 시작
	GetWorldTimerManager().SetTimer(FadeTimerHandle, this, &AUPMonsterBase::UpdateFadeOut, FadeInterval, true);

	// 타이머를 종료하기 위한 Delay
	GetWorldTimerManager().SetTimer(FadeEndHandle, this, &AUPMonsterBase::FinishFadeOut, FadeDuration, false);
}

void AUPMonsterBase::UpdateFadeOut()
{
	CurrentFadeAlpha -= 0.05f; // 투명도 감소 (값 조정 가능)

	if (CurrentFadeAlpha <= 0.0f)
	{
		CurrentFadeAlpha = 0.0f;
	}

	// 캐싱된 다이나믹 머티리얼에 투명도 적용
	for (UMaterialInstanceDynamic* DynamicMaterial : CachedDynamicMaterials)
	{
		if (DynamicMaterial)
		{
			DynamicMaterial->SetScalarParameterValue(FName("Opacity"), CurrentFadeAlpha);
		}
	}
}

void AUPMonsterBase::FinishFadeOut()
{
	// 타이머 정지
	GetWorldTimerManager().ClearTimer(FadeTimerHandle);

	// 액터 제거
	Destroy();
}
