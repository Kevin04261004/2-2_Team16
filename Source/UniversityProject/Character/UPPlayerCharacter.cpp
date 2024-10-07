// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/UPPlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/PhysicsControlComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/GameModeBase.h"
#include "Components/UPAfterImageComponent.h"
#include "Components/UPCameraComponent.h"
#include "Components/UPDashComponent.h"
#include "Curves/CurveFloat.h"
#include "Interface/UPGameInterface.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "Skill/Player/UPSkillManagerComponent.h"
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
	StateManager = CreateDefaultSubobject<UUPStateManager>(TEXT("StateManager"));
	InputHandler = CreateDefaultSubobject<UUPInputHandlerComponent>(TEXT("InputHandler"));
	SkillManager = CreateDefaultSubobject<UUPSkillManagerComponent>(TEXT("SkillManager"));
}

void AUPPlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	StateManager->InitializeStateMap();
	CameraComponent->Initialize(*CameraBoom, *FollowCamera);
	AfterImageComponent->Initialize();
	PhysicsControlComponent->Initialize();
	StateManager->Initialize(InputHandler);

	// Input Delegate
	InputHandler->OnCameraZoomed.AddUObject(CameraComponent, &UUPCameraComponent::ZoomCamera);
	InputHandler->OnCameraLookInputed.AddUObject(CameraComponent, &UUPCameraComponent::LookCamera);
}

void AUPPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	SkillManager->Initialize(Weapon, &CurAttackDamage);
	StateManager->InitializeStates(EPlayerStateType::Idle);
	SkillManager->CreateDefaultObjectSkill();

	PlayerController = Cast<AUPPlayerController>(GetController());
	check(PlayerController != nullptr);
	EnableInput(PlayerController);

	InputHandler->SetMappingContext(PlayerController);
	
	Weapon->OnWeaponHit.AddUObject(CameraComponent, &UUPCameraComponent::ShakeCamera);
}

void AUPPlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	StateManager->UpdateState();
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
	if (EnhancedInputComponent != nullptr)
	{
		InputHandler->BindActions(EnhancedInputComponent);
	}
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
	FVector ActorLocation;

	// TODO: 무기 길이 구해서 300 말고 길이 넣기.
	if (!TryCheckForwardCollision(200, OutHit, ActorLocation))
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

