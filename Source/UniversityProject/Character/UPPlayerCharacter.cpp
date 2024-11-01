// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/UPPlayerCharacter.h"

#include "EngineUtils.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/PhysicsControlComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/GameModeBase.h"
#include "Components/UPAfterImageComponent.h"
#include "Components/UPCameraComponent.h"
#include "Curves/CurveFloat.h"
#include "Interface/UPGameInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "Manager/UPPostProcessManager.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "Skill/Player/UPSkillManagerComponent.h"
#include "State/UPPlayerTakeDownState.h"
#include "UI/UPHudWidget.h"
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

	MovementComponent->bEnablePhysicsInteraction = true;
	MovementComponent->bPushForceUsingZOffset = false;
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
	Weapon->OnWeaponHit.AddUObject(CameraComponent, &UUPCameraComponent::HitShakeCamera);
	
}

void AUPPlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	StateManager->UpdateState();
	SetLookAtAlpha();
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

FVector AUPPlayerCharacter::GetLastInputVector() const
{
	return MovementComponent->GetLastInputVector();
}

void AUPPlayerCharacter::CreateAfterImage() // IUPAfterImageableInterface
{
	AfterImageComponent->CreateAfterImage();
}

float AUPPlayerCharacter::UPTakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	float super = Super::UPTakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	check(CameraComponent != nullptr);

	CameraComponent->DamagedShakeCamera();
	
	UUPPostProcessManager* PostProcessManager = GetGameInstance()->GetSubsystem<UUPPostProcessManager>();
	PostProcessManager->TogglePostProcessMaterial(EPostProcessMaterialType::EdgeFadeDesaturation, true, 0.1f);
	
	return super;
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

void AUPPlayerCharacter::ResetAttackedActorList()
{
	Weapon->ClearAttackedActors();
}


bool AUPPlayerCharacter::CanJumpInternal_Implementation() const
{
	return MovementComponent->IsMovingOnGround();
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

void AUPPlayerCharacter::SetupHUDWidget(UUPHudWidget* InHUDWidget)
{
	if (InHUDWidget)
	{
		AUPPettuCharacter* Pettu = nullptr;
		for (TActorIterator<AUPPettuCharacter> It(GetWorld()); It; ++It)
		{
			Pettu = *It;
			if (Pettu)
			{
				InHUDWidget->SetPettuCharacter(Pettu);
			}
		}
		InHUDWidget->SetPlayerCharacter(this);
	}
}

void AUPPlayerCharacter::SetLookAtAlpha()
{
	if (PettuCharacter)
	{
		FVector PlayerLocation = GetActorLocation();
		FVector PettuLocation = PettuCharacter->GetActorLocation();
		FRotator LookAtRotator = UKismetMathLibrary::FindLookAtRotation(PlayerLocation, PettuLocation);
		FRotator PlayerRotator = GetActorRotation();
		LookAtAlpha = FMath::Abs(PlayerRotator.Yaw - LookAtRotator.Yaw) > LookAtFOV ? 0.0f : 1.0f;

		LookAtLocation = PettuCharacter->GetMesh()->GetSocketLocation(LookAtLocationSocketName);
	}
}

