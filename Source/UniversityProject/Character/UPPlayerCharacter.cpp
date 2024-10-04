// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/UPPlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Camera/CameraComponent.h"
#include "Components/PhysicsControlComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/GameModeBase.h"
#include "Components/UPAfterImageComponent.h"
#include "Components/UPCharacterMovementComponent.h"
#include "Components/UPDashComponent.h"
#include "Curves/CurveFloat.h"
#include "Interface/UPGameInterface.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "Weapon/UPPlayerCharacterWeapon.h"

AUPPlayerCharacter::AUPPlayerCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
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
	
	// CDO
	AfterImageComponent = CreateDefaultSubobject<UUPAfterImageComponent>(TEXT("AfterImage"));
	PhysicsControlComponent = CreateDefaultSubobject<UPhysicsControlComponent>(TEXT("PhysicsControl"));
	DashComponent = CreateDefaultSubobject<UUPDashComponent>(TEXT("DashComponent"));
}

void AUPPlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	CameraComponent->Initialize(*CameraBoom, *FollowCamera);
	AfterImageComponent->Initialize();
	PhysicsControlComponent->Initialize();
	DashComponent->Initialize(MovementComponent, this);
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

	}
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

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AUPPlayerCharacter::MoveInputAction);
	EnhancedInputComponent->BindAction(RapidAttackAction, ETriggerEvent::Triggered, this, &AUPPlayerCharacter::RapidAttackInputAction);
	EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &AUPPlayerCharacter::DashInputAction);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AUPPlayerCharacter::JumpInputAction);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AUPPlayerCharacter::LookInputAction);
	EnhancedInputComponent->BindAction(CameraZoomAction, ETriggerEvent::Triggered, this, &AUPPlayerCharacter::ZoomCameraInputAction);
}

void AUPPlayerCharacter::MoveInputAction(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void AUPPlayerCharacter::LookInputAction(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AUPPlayerCharacter::RapidAttackInputAction(const FInputActionValue& Value)
{
	
}

void AUPPlayerCharacter::DashInputAction(const FInputActionValue& Value)
{
	DashComponent->Dash();
}

void AUPPlayerCharacter::JumpInputAction(const FInputActionValue& Value)
{
	bool Jump = Value.Get<bool>();
	GetCharacterMovement()->DoJump(Jump);
}

void AUPPlayerCharacter::WalkInputAction(const FInputActionValue& Value)
{
	MovementComponent->SetIsSprinting(false);
}

void AUPPlayerCharacter::ZoomCameraInputAction(const FInputActionValue& Value)
{
	float zoomAxis = Value.Get<float>();
	CameraComponent->ZoomCamera(zoomAxis);
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
}

void AUPPlayerCharacter::GoForward() // IUPCharacterGoForwardInterface
{
	IUPCharacterGoForwardInterface::GoForward();
	FHitResult OutHit;
	
	if (!TryCheckForwardCollision(GoForwardDistance / 2.5f, OutHit))
	{
		PhysicsControlComponent->GoForward(GoForwardDistance);
	}
}

void AUPPlayerCharacter::SetupStimuliSource()
{
	StimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("StimuliSource"));
	if (StimuliSource)
	{
		StimuliSource->RegisterForSense(TSubclassOf<UAISense_Sight>());
		StimuliSource->RegisterWithPerceptionSystem();
	}
}

