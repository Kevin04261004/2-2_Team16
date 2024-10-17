#include "UPPlayerCharacterWeapon.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/UPPostProcessManager.h"
#include "Physics/Collision.h"

// Sets default values
AUPPlayerCharacterWeapon::AUPPlayerCharacterWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	// 스켈레탈 메쉬 초기화
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;

	CollisionSocketNameArray.Add(TEXT("1"));
	CollisionSocketNameArray.Add(TEXT("2"));
	CollisionSocketNameArray.Add(TEXT("3"));

	SocketLocationArray.Init(FVector::ZeroVector, CollisionSocketNameArray.Num());
	BeforeSocketLocationArray.Init(FVector::ZeroVector, CollisionSocketNameArray.Num());
}

void AUPPlayerCharacterWeapon::CheckAttackRange()
{
	CheckCollisionSockets();
}

void AUPPlayerCharacterWeapon::ClearAttackedActors()
{
	Super::ClearAttackedActors();
	for (int32 i = 0; i < CollisionSocketNameArray.Num(); i++)
	{
		check(WeaponMesh->DoesSocketExist(CollisionSocketNameArray[i]));
		check(SocketLocationArray.IsValidIndex(i));
		check(BeforeSocketLocationArray.IsValidIndex(i));
		BeforeSocketLocationArray[i] = FVector::ZeroVector;
		SocketLocationArray[i] = FVector::ZeroVector;
	}
}
void AUPPlayerCharacterWeapon::CheckCollisionSockets()
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
    if (BeforeSocketLocationArray.IsValidIndex(0) && BeforeSocketLocationArray[0] == FVector::ZeroVector)
    {
        return;
    }

    // 보간된 위치를 기반으로 충돌 감지
    static int NUM_STEP = 5;
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

		FCollisionQueryParams weaponCollisionParams = FCollisionQueryParams(FName(TEXT("WeaponTrace")), false, GetOwner());
        // 라인 트레이스
        bool bHit1 = GetWorld()->LineTraceSingleByChannel(
            HitResult1,
            InterpolatedPosition0,
            InterpolatedPosition1,
            CCHANEL_UPACTION,
            weaponCollisionParams
        );

        bool bHit2 = GetWorld()->LineTraceSingleByChannel(
            HitResult2,
            InterpolatedPosition1,
            InterpolatedPosition2,
            CCHANEL_UPACTION,
            weaponCollisionParams
        );

        bool bHit3 = GetWorld()->LineTraceSingleByChannel(
            HitResult3,
            InterpolatedPosition2,
            InterpolatedPosition0,
            CCHANEL_UPACTION,
            weaponCollisionParams
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

void AUPPlayerCharacterWeapon::AttackSuccess(FHitResult& result, IUPDamageableInterface* Damageable)
{
	Super::AttackSuccess(result, Damageable);
	
	/* Game Time Stop */
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), StopTimeVolume);

	float StopTime = StopTimeDuration * StopTimeVolume;
	GetOwner()->GetWorldTimerManager().SetTimer(GlobalTimeTimerHandler, this, &AUPPlayerCharacterWeapon::ResetTimeDilation, StopTime,false);
	
	/* Volume */
	UUPPostProcessManager* PostProcessManager = GetGameInstance()->GetSubsystem<UUPPostProcessManager>();
	PostProcessManager->TogglePostProcessMaterial(EPostProcessMaterialType::Blur, true, 0.1f);
	// PostProcessManager->TogglePostProcessMaterial(EPostProcessMaterialType::SpeedLine, true, 0.1f);
	// PostProcessManager->TogglePostProcessMaterial(EPostProcessMaterialType::EdgeFadeDesaturation, true, 0.1f);
}

void AUPPlayerCharacterWeapon::ResetTimeDilation()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
}
