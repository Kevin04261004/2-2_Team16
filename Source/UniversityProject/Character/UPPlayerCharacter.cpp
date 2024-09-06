// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/UPPlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/UPComboAttackComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/GameModeBase.h"
#include "Interface/UPGameInterface.h"

AUPPlayerCharacter::AUPPlayerCharacter()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/UniversityProject/Input/Actions/IA_Move.IA_Move'"));
	check(InputActionMoveRef.Object != nullptr);
	MoveAction = InputActionMoveRef.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/UniversityProject/Input/Actions/IA_Look.IA_Look'"));
	check(InputActionLookRef.Object != nullptr);
	LookAction = InputActionLookRef.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionAttackRef(TEXT("/Script/EnhancedInput.InputAction'/Game/UniversityProject/Input/Actions/IA_Attack.IA_Attack'"));
	check(InputActionAttackRef.Object != nullptr);
	AttackAction = InputActionAttackRef.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionDashRef(TEXT("/Script/EnhancedInput.InputAction'/Game/UniversityProject/Input/Actions/IA_Dash.IA_Dash'"));
	check(InputActionDashRef.Object != nullptr);
	DashAction = InputActionDashRef.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionSprintRef(TEXT("/Script/EnhancedInput.InputAction'/Game/UniversityProject/Input/Actions/IA_Sprint.IA_Sprint'"));
	check(InputActionSprintRef.Object != nullptr);
	SprintAction = InputActionSprintRef.Object;
}

void AUPPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<AUPPlayerController>(GetController());
	check(PlayerController != nullptr);
	EnableInput(PlayerController);
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
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AUPPlayerCharacter::Look);
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
	// TODO: Dash->Dash();
}

void AUPPlayerCharacter::Sprint(const FInputActionValue& Value)
{
	// TODO: Sprint();
}
