// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/TestActor.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
ATestActor::ATestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATestActor::BeginPlay()
{
	Super::BeginPlay();
	DynamicMaterial = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(0), this);
	GetMesh()->SetMaterial(0, DynamicMaterial);
}

// Called every frame
void ATestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FVector HitLocation;
	FHitResult HitResult;
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	if (PlayerController->GetHitResultUnderCursor(ECC_Visibility, true, HitResult))
	{
		HitLocation = HitResult.ImpactPoint;
		ApplyHitEffect(HitLocation);  // 클릭된 위치에 색상 변경 적용
	}
}

void ATestActor::ApplyHitEffect(FVector HitLocation)
{
	// 타격 위치를 월드 좌표에서 로컬 좌표로 변환
	FVector LocalHitLocation = GetMesh()->GetComponentTransform().InverseTransformPosition(HitLocation);

	// 타격 위치를 기반으로 색상을 변경하는 파라미터를 설정
	if (DynamicMaterial)
	{
		DynamicMaterial->SetVectorParameterValue(FName("HitColor"), FLinearColor::Red);  // 예시로 빨간색을 적용
	}
}