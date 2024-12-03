// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/UPSkillBase.h"
#include "Character/UPCharacterBase.h"
#include "Character/UPPlayerCharacter.h"

UUPSkillBase::UUPSkillBase()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UUPSkillBase::Initialize(UUPCharacterMovementComponent* InCharacterMovementComponent, AUPPlayerCharacter* InPlayerCharacter)
{
	CharacterMovementComponent = InCharacterMovementComponent;
	PlayerCharacter = InPlayerCharacter;
}

bool UUPSkillBase::CanUseSkill()
{
	// 스킬 사용 가능 여부 판단
	if (CurCoolTime > 0.0f || bIsSkillActive)
	{
		return false;
	}

	return true;
}

void UUPSkillBase::BeginPlay()
{
	Super::BeginPlay();

	// 스킬 데이터가 존재하지 않습니다. 추가가 필요합니다.
	check(SkillData != nullptr);

	// 현재 쿨타임을 0으로 초기화.
	CurCoolTime = 0.0f;
	bIsSkillActive = false;
}

void UUPSkillBase::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurCoolTime > 0.0f)
	{
		CurCoolTime -= DeltaTime;
	}
	else
	{
		SetComponentTickEnabled(false);
	}
}

float UUPSkillBase::GetSkillAttackDamage()
{
	// 컴포넌트를 부착하고 있는 캐릭터가 UP캐릭터인지 확인합니다.
	AUPCharacterBase* character = Cast<AUPCharacterBase>(GetOwner());
	check (character != nullptr);

	// 스킬 컴포넌트를 부착해둔 캐릭터의 데미지를 기본 공격력으로 넣은 스킬 데이터의 공격력을 가져옵니다.
	return SkillData->GetSkillDamage(character->GetStat()->GetTotalStat().AttackDamage);
}

bool UUPSkillBase::TryActivateSkill(AActor* TargetOrNull)
{
	if (!CanUseSkill())
	{
		return false;
	}
	// 스킬 실행!
	CustomActivate(TargetOrNull);

	// 스킬 상태 설정
	bIsSkillActive = true;

	// 스킬 지속 시간을 설정하고 타이머를 시작 (스킬이 끝나면 DeActivate 호출)
	// 타이머 델리게이트 설정 (인자를 넘겨야 하는 경우)
	FName FunctionName = FName("CustomDeActivate");
	// 함수명이 클래스에 존재하는지 확인
	check(FindFunction(FunctionName) != nullptr);
	DeActivateSkillTimerDelegate.Unbind();
	DeActivateSkillTimerDelegate.BindUFunction(this, FunctionName, TargetOrNull);
	
	// 타이머 시작! (스킬이 끝나면 DeactivateSkill 호출, 인자가 전달됨)
	GetWorld()->GetTimerManager().SetTimer(SkillDurationHandle, DeActivateSkillTimerDelegate, GetSkillDuration(), false);
	
	return true;
}

bool UUPSkillBase::TryStopSkill()
{
	// 스킬 중지 가능 여부 판단
	if (!SkillData->CanStopSkill() || !bIsSkillActive)
	{
		return false;
	}

	CustomStop();
	
	// 스킬 종료 전에 타이머를 강제로 실행하고 싶을 때
	if (GetWorld()->GetTimerManager().IsTimerActive(SkillDurationHandle))
	{
		// 남은 시간에 관계없이 바로 DeactivateSkill 호출
		DeActivateSkillTimerDelegate.ExecuteIfBound();
		
		// 타이머 종료
		GetWorld()->GetTimerManager().ClearTimer(SkillDurationHandle);
	}

	
	return true;
}

void UUPSkillBase::CustomActivate_Implementation(AActor* TargetOrNull)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Use Skill Success"));

	AUPCharacterBase* player = Cast<AUPCharacterBase>(GetOwner());
	UAnimMontage* skillAnimation = SkillData->GetSkillAnimation();
	if (player != nullptr && skillAnimation != nullptr)
	{
		UAnimInstance* AnimInstance = player->GetMesh()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			float animSpeed = SkillData->GetAnimationSpeed(player->GetStat()->GetTotalStat().AttackSpeed);
			AnimInstance->Montage_Play(skillAnimation, animSpeed);
		}
	}

	SetOwnerMovementMode(MOVE_None);
}

void UUPSkillBase::CustomStop_Implementation()
{
	// 스킬 애니메이션이 실행중이면 강제로 종료.
	AUPCharacterBase* Character = Cast<AUPCharacterBase>(GetOwner());
	if (Character)
	{
		UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
		if (AnimInstance && SkillData->GetSkillAnimation() != nullptr) // 스킬과 연결된 몽타주가 있을 때
		{
			// 스킬 애니메이션이 실행 중이면 강제로 종료.
			if (AnimInstance->Montage_IsPlaying(SkillData->GetSkillAnimation()))
			{
				// 애니메이션 몽타주를 강제로 중지.
				AnimInstance->Montage_Stop(0.2f, SkillData->GetSkillAnimation());  // 0.2초 동안 페이드를 적용하면서 중단
			}
		}
	}
}

void UUPSkillBase::CustomDeActivate_Implementation(AActor* TargetOrNull)
{
	CurCoolTime = SkillData->GetCoolTime();
	SetComponentTickEnabled(true);
	GetWorld()->GetTimerManager().ClearTimer(SkillDurationHandle);
	SetOwnerMovementMode(MOVE_Walking);
	
	bIsSkillActive = false;
}

float UUPSkillBase::GetSkillDuration() const
{
	AUPCharacterBase* character = Cast<AUPCharacterBase>(GetOwner());
	float skillDuration = SkillData->GetSkillDuration(character->GetStat()->GetTotalStat().AttackSpeed);
	return skillDuration;
}

void UUPSkillBase::SetOwnerMovementMode(EMovementMode mode)
{
	AUPCharacterBase* character = Cast<AUPCharacterBase>(GetOwner());
	if (character != nullptr)
	{
		character->MovementComponent->MovementMode = mode;
	}
}
