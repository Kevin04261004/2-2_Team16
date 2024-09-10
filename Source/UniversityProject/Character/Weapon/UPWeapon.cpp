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

	CollisionSocketNameArray.Add(TEXT("1"));
	CollisionSocketNameArray.Add(TEXT("2"));
	CollisionSocketNameArray.Add(TEXT("3"));

	SocketLocationArray.Init(FVector(0,0,0), CollisionSocketNameArray.Num());
	BeforeSocketLocationArray.Init(FVector(0,0,0), CollisionSocketNameArray.Num());
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

void AUPWeapon::NotifyAttackCheck()
{
	CheckCollisionSockets();
}

void AUPWeapon::NotifyAttackEnd()
{
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
	// TODO: 한번 휘두를때 이미 맞은 상대는 더 안 맞게 수정.
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("충돌!!"), true, FVector2D(1.5f, 1.5f));
	}
}

void AUPWeapon::CheckCollisionSockets()
{
	check(WeaponMesh != nullptr);

	// 소켓 위치 가져오기. (Array에 넣은 순서대로)
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
	
	// 디버그
	DrawDebugLine(GetWorld(), SocketLocationArray[2], SocketLocationArray[0], FColor::Green, false, 0.1f, 0, 2.0f);

	// 보간된 위치를 기반으로 충돌 감지
	static int NUM_STEP = 5;
	for (int Step = 0; Step < NUM_STEP; Step++)
	{
		float Alpha = (float)Step / NUM_STEP;

		// 소켓 위치 보간
		FVector InterpolatedPosition0 = FMath::Lerp(BeforeSocketLocationArray[0], SocketLocationArray[0], Alpha);
		FVector InterpolatedPosition1 = FMath::Lerp(BeforeSocketLocationArray[1], SocketLocationArray[1], Alpha);
		FVector InterpolatedPosition2 = FMath::Lerp(BeforeSocketLocationArray[2], SocketLocationArray[2], Alpha);

		/* 게임의 특성상 보스와 1대1이기에 굳이 Multicast를 하지 않고 조금이라도 가벼운 Single로 진행했습니다. */
		// 라인 트레이스 수행 (소켓 위치를 연결하여 트레이스)
		FHitResult HitResult;
        
		// 첫 번째 소켓과 두 번째 소켓 사이에 라인 트레이스
		bool bHit1 = GetWorld()->LineTraceSingleByChannel(
			HitResult,
			InterpolatedPosition0,
			InterpolatedPosition1,
			ECC_Visibility,
			FCollisionQueryParams(FName(TEXT("WeaponTrace")), false, this)
		);

		// 두 번째 소켓과 세 번째 소켓 사이에 라인 트레이스
		bool bHit2 = GetWorld()->LineTraceSingleByChannel(
			HitResult,
			InterpolatedPosition1,
			InterpolatedPosition2,
			ECC_Visibility,
			FCollisionQueryParams(FName(TEXT("WeaponTrace")), false, this)
		);

		// 세 번째 소켓과 첫 번째 소켓 사이에 라인 트레이스
		bool bHit3 = GetWorld()->LineTraceSingleByChannel(
			HitResult,
			InterpolatedPosition2,
			InterpolatedPosition0,
			ECC_Visibility,
			FCollisionQueryParams(FName(TEXT("WeaponTrace")), false, this)
		);

		if (bHit1 || bHit2 || bHit3)
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit detected in line trace!"));
			Attack(HitResult);
		}

		// 디버그로 라인 그리기
		DrawDebugLine(GetWorld(), InterpolatedPosition0, InterpolatedPosition1, FColor::Green, false, 0.3f, 0, 2.0f);
		DrawDebugLine(GetWorld(), InterpolatedPosition1, InterpolatedPosition2, FColor::Green, false, 0.3f, 0, 2.0f);
		DrawDebugLine(GetWorld(), InterpolatedPosition2, InterpolatedPosition0, FColor::Green, false, 0.3f, 0, 2.0f);
	}
	
}
