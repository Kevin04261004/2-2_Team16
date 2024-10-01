// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/UPPlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "MovieSceneTracksComponentTypes.h"
#include "Camera/CameraComponent.h"
#include "Components/AutoTargetingComponent.h"
#include "Components/PhysicsControlComponent.h"
#include "Components/UPComboAttackComponent.h"
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
	ComboAttack = CreateDefaultSubobject<UUPComboAttackComponent>(TEXT("Combo Attack"));
	AfterImageComponent = CreateDefaultSubobject<UUPAfterImageComponent>(TEXT("AfterImage"));
	PhysicsControlComponent = CreateDefaultSubobject<UPhysicsControlComponent>(TEXT("PhysicsControl"));
	DashComponent = CreateDefaultSubobject<UUPDashComponent>(TEXT("DashComponent"));
	AutoTargetingComponent = CreateDefaultSubobject<UAutoTargetingComponent>(TEXT("AutoTargeting"));

	// Skill
	InitSkillMap();
}

void AUPPlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	CameraComponent->Initialize(*CameraBoom, *FollowCamera);
	AfterImageComponent->Initialize(*this);
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
		ComboAttack->OnComboAttackFinish.AddUObject(PlayerWeapon, &AUPPlayerCharacterWeapon::ComboStepEnd);
		ComboAttack->OnComboStepEnd.AddUObject(PlayerWeapon, &AUPPlayerCharacterWeapon::ComboStepEnd);
	}

	CreateDefaultObjectSkill();
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
	EnhancedInputComponent->BindAction(HeavyAttackAction, ETriggerEvent::Triggered, this, &AUPPlayerCharacter::HeavyAttackInputAction);
	EnhancedInputComponent->BindAction(ESkillAction, ETriggerEvent::Triggered, this, &AUPPlayerCharacter::ESkillInputAction);
	EnhancedInputComponent->BindAction(RSkillAction, ETriggerEvent::Triggered, this, &AUPPlayerCharacter::RSkillInputAction);
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
	UseSkill(ESkillType::RapidAttack01);
}

void AUPPlayerCharacter::HeavyAttackInputAction(const FInputActionValue& Value)
{
	UseSkill(ESkillType::RapidAttack02);
}

void AUPPlayerCharacter::ESkillInputAction(const FInputActionValue& Value)
{
	UseSkill(ESkillType::ESkill);
}

void AUPPlayerCharacter::RSkillInputAction(const FInputActionValue& Value)
{
	UseSkill(ESkillType::RSkill);
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

void AUPPlayerCharacter::InitSkillMap()
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ESkillType"), true);
	if (!EnumPtr)
	{
		return;
	}
	
	for (int32 i = 0; i < EnumPtr->NumEnums() - 1; ++i)
	{
		if (!EnumPtr->HasMetaData(TEXT("Hidden"), i))
		{
			ESkillType EnumValue = static_cast<ESkillType>(EnumPtr->GetValueByIndex(i));
			SkillMapInitializer.Add(EnumValue, nullptr);
		}
	}
}

void AUPPlayerCharacter::CreateDefaultObjectSkill()
{
	for (TTuple<ESkillType, TSubclassOf<UUPSkillBase>> skillMapTuple : SkillMapInitializer)
	{
		if (skillMapTuple.Value == nullptr)
		{
			continue;
		}
		if (UUPSkillBase* NewSkillComponent = NewObject<UUPSkillBase>(this, skillMapTuple.Value))
		{
			// 컴포넌트를 월드에 등록합니다.
			NewSkillComponent->RegisterComponent();

			// SkillMap에 새로 생성된 컴포넌트를 추가합니다.
			ESkillType Type = skillMapTuple.Key;
			SkillMap.Add(Type, NewSkillComponent);

			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Skill Map Created"));
		}
	}
}

void AUPPlayerCharacter::UseSkill(ESkillType skillType)
{
	UUPSkillBase** skill_pp = SkillMap.Find(skillType);
	if (skill_pp == nullptr)
	{
		return;
	}
	UUPSkillBase* skill = *skill_pp;
	if (skill == nullptr)
	{
		return;
	}
	
	// 타겟팅, 오토타겟팅이면 상대를 바라보게!
	AActor* Target = nullptr;
	if (skill->CanUseSkill() && skill->GetSkillData()->IsAutoTargetingSkill())
	{
		Target = AutoTargetingComponent->FindDamageableTargetOrNull(GetActorLocation(), EAutoTargetingMode::ATM_Nearest);

		if (Target != nullptr)
		{
			FVector TargetLocation = Target->GetActorLocation();
			AutoTargetingComponent->RotateToTarget(TargetLocation);
		}
	}
	if (skill->CanUseSkill())
	{
		// TODO: 모듈화
		AUPPlayerCharacterWeapon* playerWeapon = Cast<AUPPlayerCharacterWeapon>(Weapon);
		playerWeapon->ComboStepEnd();
		CurAttackDamage = skill->GetSkillData()->GetSkillDamage(StatComponent->GetTotalStat().AttackDamage);
		skill->TryActivateSkill(Target);
	}
}
