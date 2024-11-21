// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/UPEnemyMonster.h"
#include "BrainComponent.h"
#include "AI/UPPettuAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Weapon/UPPettuWeapon.h"
#include "Components/TimelineComponent.h"

AUPEnemyMonster::AUPEnemyMonster(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UUPCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	static ConstructorHelpers::FObjectFinder<UUPCharacterStatData> StatDataRef(TEXT("/Game/UniversityProject/GameData/DA_EnemyMonsterStat.DA_EnemyMonsterStat"));
	StatComponent->SetBaseStat(StatDataRef.Object.Get()->Stat);

	bIsDead = false;

	InitSkillMap();
	DiedCondition = EStageConditionType::KillMonster;
}

void AUPEnemyMonster::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	MonsterAIController = Cast<AUPPettuAIController>(GetController());
}

void AUPEnemyMonster::BeginPlay()
{
	Super::BeginPlay();
	bIsKnockbackActive = false;
	MovementComponent->StopMovementImmediately();
	MovementComponent->SetMovementMode(MOVE_Walking);
}

float AUPEnemyMonster::UPTakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	if (DamageCauser && !bIsKnockbackActive)
	{
		bIsKnockbackActive = true;
		if (MonsterAIController && MonsterAIController->GetBlackboardComponent())
		{
			MonsterAIController->GetBlackboardComponent()->SetValueAsBool("IsKnockback", bIsKnockbackActive);
			MonsterAIController->StopMovement();
		}
		MovementComponent->DisableMovement();

		// 공격한 대상을 바라보게 설정
		FRotator LookAtRotation = (DamageCauser->GetActorLocation() - GetActorLocation()).Rotation();
		LookAtRotation.Pitch = 0;
		SetActorRotation(LookAtRotation);

		// 현재 캐릭터의 위치와 데미지 가한 액터의 위치를 가져옴
		FVector MyLocation = GetActorLocation();
		FVector DamageCauserLocation = DamageCauser->GetActorLocation();

		// 넉백 방향 계산 (데미지 가한 액터의 반대 방향)
		FVector KnockbackDirection = (MyLocation - DamageCauserLocation).GetSafeNormal();
		KnockbackDirection.Z = 0;

		// 넉백 세기 (적절히 조정 필요)
		FVector KnockbackTarget = MyLocation + (KnockbackDirection * KnockbackStrength);

		// 타임라인을 이용한 넉백 애니메이션 적용
		FOnTimelineFloat ProgressFunction;
		ProgressFunction.BindUFunction(this, FName("KnockbackProgress"));
		FOnTimelineEvent FinishedFunction;
		FinishedFunction.BindUFunction(this, FName("KnockbackFinished"));

		if (!KnockbackTimeline)
		{
			KnockbackTimeline = NewObject<UTimelineComponent>(this, FName("KnockbackTimeline"));
			KnockbackTimeline->AddInterpFloat(KnockbackCurve, ProgressFunction);
			KnockbackTimeline->SetTimelineFinishedFunc(FinishedFunction);
			KnockbackTimeline->RegisterComponent();
		}

		KnockbackStartLocation = MyLocation;
		KnockbackEndLocation = KnockbackTarget;
		KnockbackTimeline->PlayFromStart();
	}
	return Super::UPTakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void AUPEnemyMonster::SetDead()
{
	Super::SetDead();
}

void AUPEnemyMonster::DeadAnimEnd(UAnimMontage* Montage, bool bInterrupted)
{
	Super::DeadAnimEnd(Montage, bInterrupted);
}

void AUPEnemyMonster::PlayPatternMontage(UAnimMontage* Montage)
{
	PatternMontage = Montage;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance == nullptr)
	{
		return;
	}
	AnimInstance->StopAllMontages(0.0f);
	if (PatternMontage)
	{
		AnimInstance->Montage_Play(PatternMontage, 1.0f);
		AnimInstance->OnMontageEnded.AddDynamic(this, &AUPEnemyMonster::PatternMontageEnd);
	}
}

void AUPEnemyMonster::PatternMontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	GetMesh()->GetAnimInstance()->OnMontageEnded.RemoveDynamic(this, &AUPEnemyMonster::PatternMontageEnd);
}


void AUPEnemyMonster::AttackHitCheck()
{
}

void AUPEnemyMonster::AttackHitCheck(bool bIsAttached, FName SocketName, USkeletalMeshComponent* MeshComp,
                                     float AttackRange, float Amount, FVector CollisionLocation)
{
	check(Weapon != nullptr);
	AUPPettuWeapon* MonsterWeapon = Cast<AUPPettuWeapon>(Weapon);
	if (MonsterWeapon && bIsAttached && SocketName != NAME_None)
	{
		MonsterWeapon->CheckAttackRange(SocketName, MeshComp, AttackRange, CollisionLocation);
	}
	else if (MonsterWeapon && !bIsAttached)
	{
		MonsterWeapon->CheckAttackRange(AttackRange, Amount, CollisionLocation);
	}
}

void AUPEnemyMonster::KnockbackProgress(float Value)
{
	FVector NewLocation = FMath::Lerp(KnockbackStartLocation, KnockbackEndLocation, Value);
	SetActorLocation(NewLocation);
}

void AUPEnemyMonster::KnockbackFinished()
{
	bIsKnockbackActive = false;
	MovementComponent->SetMovementMode(MOVE_Walking);
	AUPPettuAIController* AIController = Cast<AUPPettuAIController>(GetController());
	if (AIController && AIController->GetBlackboardComponent())
	{
		AIController->GetBlackboardComponent()->SetValueAsBool("IsKnockback", bIsKnockbackActive);
	}
}

