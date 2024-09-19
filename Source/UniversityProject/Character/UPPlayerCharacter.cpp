// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/UPPlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "AfterImage/UPAfterImage.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/UPComboAttackComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/GameModeBase.h"
#include "Components/TimelineComponent.h"
#include "Components/UPAfterImageComponent.h"
#include "Components/UPCharacterMovementComponent.h"
#include "Curves/CurveFloat.h"
#include "Interface/UPGameInterface.h"
#include "Weapon/UPPlayerCharacterWeapon.h"

AUPPlayerCharacter::AUPPlayerCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	/* Init Components */
	// Camera Setting
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	CameraComponent = CreateDefaultSubobject<UUPCameraComponent>(TEXT("CameraComponent"));
	
	// 
	static ConstructorHelpers::FObjectFinder<UCurveFloat> CurveRef(TEXT("/Game/UniversityProject/GameData/CV_DashCurve"));
	if (CurveRef.Succeeded())
	{
		DashCurve = CurveRef.Object;
	}
	
	if (DashCurve)
	{
		TimelineCallback.BindUFunction(this, FName("UpdateDash"));
		TimelineFinishedCallback.BindUFunction(this, FName("FinishDash"));

		DashTimeline.SetTimelineFinishedFunc(TimelineFinishedCallback);
		DashTimeline.AddInterpFloat(DashCurve, TimelineCallback);

		UE_LOG(LogTemp, Warning, TEXT("Dash Timeline Initialized with Curve"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Dash Curve is not loaded"));
	}

	DashDistance = 500.0f;
	// Set Combo
	ComboAttack = CreateDefaultSubobject<UUPComboAttackComponent>(TEXT("Combo Attack"));
	AfterImageComponent = CreateDefaultSubobject<UUPAfterImageComponent>(TEXT("AfterImage"));
}

void AUPPlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	CameraComponent->Initialize(*CameraBoom, *FollowCamera);
	AfterImageComponent->Initialize(*this);
}

void AUPPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<AUPPlayerController>(GetController());
	check(PlayerController != nullptr);
	EnableInput(PlayerController);

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(IMC_BackView, 0);
	}
	Weapon->OnWeaponHit.AddUObject(CameraComponent, &UUPCameraComponent::ShakeCamera);

	/* Actor Delegate */
	AUPPlayerCharacterWeapon* PlayerWeapon = Cast<AUPPlayerCharacterWeapon>(Weapon);
	if (PlayerWeapon)
	{
		ComboAttack->OnComboAttackFinish.AddUObject(PlayerWeapon, &AUPPlayerCharacterWeapon::ComboStepEnd);
		ComboAttack->OnComboStepEnd.AddUObject(PlayerWeapon, &AUPPlayerCharacterWeapon::ComboStepEnd);
	}
	
	CollisionComponent = Cast<UPrimitiveComponent>(GetRootComponent());
}

void AUPPlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// 타임라인 프레임 당 업데이트하고 변경 점 반영함
	DashTimeline.TickTimeline(DeltaSeconds);
}

void AUPPlayerCharacter::SetDead()
{
	Super::SetDead();
	check(PlayerController != nullptr);
	DisableInput(PlayerController);
	IUPGameInterface* UPGameMode = Cast<IUPGameInterface>(GetWorld()->GetAuthGameMode());
	UPGameMode->OnPlayerDead();
}

void AUPPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AUPPlayerCharacter::Move);
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AUPPlayerCharacter::Attack);
	EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &AUPPlayerCharacter::Dash);
	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &AUPPlayerCharacter::Sprint);
	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AUPPlayerCharacter::Walk);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AUPPlayerCharacter::Look);
	EnhancedInputComponent->BindAction(CameraZoomAction, ETriggerEvent::Triggered, this, &AUPPlayerCharacter::ZoomCamera);
}

void AUPPlayerCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void AUPPlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AUPPlayerCharacter::Attack(const FInputActionValue& Value)
{
	ComboAttack->ProcessComboCommand();
}

void AUPPlayerCharacter::Dash(const FInputActionValue& Value)
{
	FVector LastInputVector = GetCharacterMovement()->GetLastInputVector();
	FVector PlayerLocation = GetActorLocation();
	FVector TraceDirectionVector = PlayerLocation + (LastInputVector * DashDistance);
	if (UAIBlueprintHelperLibrary::IsValidAIDirection(LastInputVector))
	{
		FHitResult HitResult;
		bool bHit = GetWorld()->LineTraceSingleByChannel(
			HitResult,
			PlayerLocation,
			TraceDirectionVector,
			ECC_Visibility,
			FCollisionQueryParams(FName(TEXT("DashTrace")), false, this)
		);
		DrawDebugLine(GetWorld(), PlayerLocation, TraceDirectionVector, FColor::Red, false, 2.0f, 0, 2.0f);
		UE_LOG(LogTemp, Warning, TEXT("Dash Trace"));
		if (bHit)
		{
			FVector DashDirection = HitResult.Location;
			DashStart(DashDirection, LastInputVector);
		}
		else
		{
			FVector DashDirection = LastInputVector + HitResult.TraceEnd;
			DashStart(DashDirection, LastInputVector);
		}
	}
	else
	{
		FHitResult HitResult;
		TraceDirectionVector = PlayerLocation + (GetActorForwardVector() * DashDistance);
		bool bHit = GetWorld()->LineTraceSingleByChannel(
			HitResult,
			PlayerLocation,
			TraceDirectionVector,
			ECC_Visibility,
			FCollisionQueryParams(FName(TEXT("DashTrace")), false, this)
		);
		DrawDebugLine(GetWorld(), PlayerLocation, TraceDirectionVector, FColor::Red, false, 2.0f, 0, 2.0f);
		if (bHit)
		{
			FVector DashDirection = HitResult.Location;
			DashStart(DashDirection, GetActorForwardVector());
		}
		else
		{
			FVector DashDirection = (GetActorForwardVector()) + HitResult.TraceEnd;
			DashStart(DashDirection, GetActorForwardVector());
		}
	}
}

void AUPPlayerCharacter::Sprint(const FInputActionValue& Value)
{
	MovementComponent->SetIsSprinting(true);
}

void AUPPlayerCharacter::Walk(const FInputActionValue& Value)
{
	MovementComponent->SetIsSprinting(false);
}

void AUPPlayerCharacter::ZoomCamera(const FInputActionValue& Value)
{
	float zoomAxis = Value.Get<float>();
	CameraComponent->ZoomCamera(zoomAxis);
}

void AUPPlayerCharacter::DashStart(FVector DashDirection, FVector DashVelocity)
{
	DashStartLocation = GetActorLocation();
	DashEndLocation = DashDirection;
	DashEndVelocity = DashVelocity;
	if (!DashTimeline.IsPlaying())
	{
		DashTimeline.PlayFromStart();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Dash Timeline is already playing"));
	}
}

void AUPPlayerCharacter::UpdateDash(float Value)
{
	//위치 보간
	FVector NewLocation = FMath::Lerp(DashStartLocation, DashEndLocation, Value);
	SetActorLocation(NewLocation);
	
}

void AUPPlayerCharacter::FinishDash()
{
	GetCharacterMovement()->Velocity = DashEndVelocity * 500.0f;
}

void AUPPlayerCharacter::CreateAfterImage() // IUPAfterImageableInterface
{
	AfterImageComponent->CreateAfterImage();
}

void AUPPlayerCharacter::AttackHitCheck() // IUPAnimationAttackCheckInterface
{
	check(Weapon != nullptr);
	AUPPlayerCharacterWeapon* PlayerWeapon = Cast<AUPPlayerCharacterWeapon>(Weapon);
	if (PlayerWeapon)
	{
		PlayerWeapon->CheckAttackRange();
	}
	// Weapon->CheckAttackRange();
}

void AUPPlayerCharacter::GoForward() // IUPCharacterGoForwardInterface
{
	IUPCharacterGoForwardInterface::GoForward();
	
	// SimulatePhysics가 true이면, 캐릭터가 인풋으로 이동이 불가능함. 그래서, 0.2초정도 활성화 후 되돌리기.
	CollisionComponent->SetSimulatePhysics(true);
	if (!TryCheckForwardCollision(GoForwardDistance / 2.5f))
	{
		CollisionComponent->AddImpulse(GetActorForwardVector() * GoForwardDistance,"", true);
	}
	GetWorld()->GetTimerManager().SetTimer(PhysicsTimerHandle, this, &AUPPlayerCharacter::SetPhysicsFalse, 0.2f, false);
}

void AUPPlayerCharacter::SetPhysicsFalse()
{
	if (CollisionComponent != nullptr)
	{
		CollisionComponent->SetSimulatePhysics(false);
	}
}
