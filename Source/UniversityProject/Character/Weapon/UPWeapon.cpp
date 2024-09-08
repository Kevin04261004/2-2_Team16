#include "UPWeapon.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"

// Sets default values
AUPWeapon::AUPWeapon()
{
	// 틱 안씀
	PrimaryActorTick.bCanEverTick = false;

	// 스켈레탈 메쉬 초기화
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;
}

void AUPWeapon::PerformSwing(int32 CurrentFrameIndex)
{
	// 궤적 데이터가 있고, 현재 프레임 데이터가 존재하는지 확인
	if (SwingTrajectoryData && SwingTrajectoryData->SwingFrameData.IsValidIndex(CurrentFrameIndex))
	{
		// 현재 프레임 데이터를 가져옴 읽기전용, 구조체라서 참조로 가져옴
		const FWeaponSwingFrameData& FrameData = SwingTrajectoryData->SwingFrameData[CurrentFrameIndex];

		// 트레이스로 충돌을 감지
		FHitResult HitResult;
		FVector Start = FrameData.StartPosition;
		FVector End = FrameData.EndPosition;

		// 라인 트레이스 사용 가벼우니깐.. Single과 Multi 둘 중 어느것을 사용할지는 추후 게임성따라 결정 일단은 Single
		// 트레이스 채널 하나 만들어야됨 WeaponTrace라는 이름으로
		bool bHit = GetWorld()->LineTraceSingleByChannel(
			HitResult,
			Start,
			End,
			ECC_Visibility,
			FCollisionQueryParams(FName(TEXT("WeaponTrace")), false, this)
		);

		// 충돌이 감지된 경우 처리
		if (bHit)
		{
			AActor* HitActor = HitResult.GetActor();
			if (HitActor)
			{
				UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitActor->GetName());
				// 나중에 로직 구현
			}
		}

		// 충돌 범위 디버그
		DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 2.0f);
	}
}