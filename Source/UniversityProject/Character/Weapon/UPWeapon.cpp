#include "UPWeapon.h"
#include "DrawDebugHelpers.h"
#include "Interface/UPDamageableInterface.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AUPWeapon::AUPWeapon()
{
	// 틱 안씀
	PrimaryActorTick.bCanEverTick = false;

	// 스켈레탈 메쉬 초기화
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;

	CollisionSocketNameArray.Add(TEXT("1"));
	CollisionSocketNameArray.Add(TEXT("2"));
	CollisionSocketNameArray.Add(TEXT("3"));

	SocketLocationArray.Init(FVector(0,0,0), CollisionSocketNameArray.Num());
	BeforeSocketLocationArray.Init(FVector(0,0,0), CollisionSocketNameArray.Num());

	// Set Effect
	// static ConstructorHelpers::FObjectFinder<UParticleSystem> HitEffectRef(TEXT("/Game/Assets/InfinityBladeEffects/Effects/FX_Combat_Base/Impact/P_ImpactSpark.P_ImpactSpark"));
	// check(HitEffectRef.Object != nullptr);
	// HitEffect = HitEffectRef.Object;
}

void AUPWeapon::CheckAttackRange()
{
	CheckCollisionSockets();
}

void AUPWeapon::ComboStepEnd()
{
	AttackedActors.Empty();
	for (int32 i = 0; i < CollisionSocketNameArray.Num(); i++)
	{
		check(WeaponMesh->DoesSocketExist(CollisionSocketNameArray[i]));
		check(SocketLocationArray.IsValidIndex(i));
		check(BeforeSocketLocationArray.IsValidIndex(i));
		BeforeSocketLocationArray[i] = FVector(0, 0, 0);
		SocketLocationArray[i] = FVector(0, 0, 0);
	}
}

void AUPWeapon::Attack(FHitResult& result)
{
	if (AttackedActors.Contains(result.GetActor()))
	{
		return;
	}
	AttackedActors.Add(result.GetActor());
	
	IUPDamageableInterface* Damageable = Cast<IUPDamageableInterface>(result.GetActor());
	if (Damageable == nullptr)
	{
		return;
	}

	/* Effect */
	UParticleSystem* DamageParticle = BaseHitEffect;
	if (Damageable->GetHitEffect() != nullptr)
	{
		DamageParticle = Damageable->GetHitEffect();
	}
	if (DamageParticle != nullptr)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DamageParticle, result.ImpactPoint);
	}
	
	/* Sound */
	USoundBase* DamageSound = BaseHitSound;
	if (Damageable->GetHitSound() != nullptr)
	{
		DamageSound = Damageable->GetHitSound();
	}
	if (DamageSound != nullptr)
	{
		UGameplayStatics::SpawnSound2D(GetWorld(), DamageSound);
	}

	OnWeaponHit.Broadcast(result);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("충돌됨"), true, FVector2D(1.5f, 1.5f));
	}
}

void AUPWeapon::CheckCollisionSockets()
{
    check(WeaponMesh != nullptr);

    // 소켓 위치 가져오기
    for (int32 i = 0; i < CollisionSocketNameArray.Num(); i++)
    {
        check(WeaponMesh->DoesSocketExist(CollisionSocketNameArray[i]));
        check(SocketLocationArray.IsValidIndex(i));
        check(BeforeSocketLocationArray.IsValidIndex(i));

        BeforeSocketLocationArray[i] = SocketLocationArray[i];
        SocketLocationArray[i] = WeaponMesh->GetSocketLocation(CollisionSocketNameArray[i]);
    }

    // 이전 위치가 초기값이면(처음이면) 리턴
    if (BeforeSocketLocationArray.IsValidIndex(0) && BeforeSocketLocationArray[0] == FVector(0,0,0))
    {
        return;
    }

    // 디버그 라인
    DrawDebugLine(GetWorld(), SocketLocationArray[2], SocketLocationArray[0], FColor::Green, false, 0.1f, 0, 2.0f);

    // 보간된 위치를 기반으로 충돌 감지
    static int NUM_STEP = 10;
    for (int Step = 0; Step < NUM_STEP; Step++)
    {
        float Alpha = (float)Step / NUM_STEP;

        // 소켓 위치 보간
        FVector InterpolatedPosition0 = FMath::Lerp(BeforeSocketLocationArray[0], SocketLocationArray[0], Alpha);
        FVector InterpolatedPosition1 = FMath::Lerp(BeforeSocketLocationArray[1], SocketLocationArray[1], Alpha);
        FVector InterpolatedPosition2 = FMath::Lerp(BeforeSocketLocationArray[2], SocketLocationArray[2], Alpha);

        // 각 트레이스마다 별도의 FHitResult 사용
        FHitResult HitResult1;
        FHitResult HitResult2;
        FHitResult HitResult3;

        // 라인 트레이스
        bool bHit1 = GetWorld()->LineTraceSingleByChannel(
            HitResult1,
            InterpolatedPosition0,
            InterpolatedPosition1,
            ECC_Visibility,
            FCollisionQueryParams(FName(TEXT("WeaponTrace")), false, this)
        );

        bool bHit2 = GetWorld()->LineTraceSingleByChannel(
            HitResult2,
            InterpolatedPosition1,
            InterpolatedPosition2,
            ECC_Visibility,
            FCollisionQueryParams(FName(TEXT("WeaponTrace")), false, this)
        );

        bool bHit3 = GetWorld()->LineTraceSingleByChannel(
            HitResult3,
            InterpolatedPosition2,
            InterpolatedPosition0,
            ECC_Visibility,
            FCollisionQueryParams(FName(TEXT("WeaponTrace")), false, this)
        );

        // 충돌된 액터가 이미 처리된 액터인지 확인 및 처리
        if (bHit1 && HitResult1.GetActor() && !AttackedActors.Contains(HitResult1.GetActor()))
        {
            Attack(HitResult1);
        }

        if (bHit2 && HitResult2.GetActor() && !AttackedActors.Contains(HitResult2.GetActor()))
        {
            Attack(HitResult2);
        }

        if (bHit3 && HitResult3.GetActor() && !AttackedActors.Contains(HitResult3.GetActor()))
        {
            Attack(HitResult3);
        }

        // 디버그 라인 그리기
        DrawDebugLine(GetWorld(), InterpolatedPosition0, InterpolatedPosition1, FColor::Green, false, 0.3f, 0, 2.0f);
        DrawDebugLine(GetWorld(), InterpolatedPosition1, InterpolatedPosition2, FColor::Green, false, 0.3f, 0, 2.0f);
        DrawDebugLine(GetWorld(), InterpolatedPosition2, InterpolatedPosition0, FColor::Green, false, 0.3f, 0, 2.0f);
    }
}
