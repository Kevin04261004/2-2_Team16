// Fill out your copyright notice in the Description page of Project Settings.


#include "State/UPPlayerCantMoveState.h"

UUPPlayerCantMoveState::UUPPlayerCantMoveState()
{
}

void UUPPlayerCantMoveState::Initialize(AUPPlayerCharacter* InOwnerCharacter,
	class UUPInputHandlerComponent* InInputHandler)
{
	Super::Initialize(InOwnerCharacter, InInputHandler);
}

void UUPPlayerCantMoveState::EnterState()
{
	Super::EnterState();
	InputHandler->OnMoveInputed.Broadcast(FVector2D::ZeroVector);
	OwnerCharacter->MovementComponent->StopMovementImmediately();
	OwnerCharacter->MovementComponent->ClearAccumulatedForces();
	OwnerCharacter->MovementComponent->Velocity = FVector::ZeroVector;
	OwnerCharacter->MovementComponent->StopActiveMovement();
	OwnerCharacter->Controller->StopMovement();

	// 이동 모드를 MOVE_None로 설정 (움직이지 않음)
	OwnerCharacter->MovementComponent->DisableMovement();
	OwnerCharacter->MovementComponent->SetMovementMode(MOVE_None);
    
	// 입력을 비활성화
	if (APlayerController* PC = OwnerCharacter->GetController<APlayerController>())
	{
		OwnerCharacter->DisableInput(PC); // 입력을 완전히 차단
	}
}

void UUPPlayerCantMoveState::ExitState()
{
	Super::ExitState();
	if (APlayerController* PC = OwnerCharacter->GetController<APlayerController>())
	{
		OwnerCharacter->EnableInput(PC); // 입력을 다시 활성화
	}
}

void UUPPlayerCantMoveState::UpdateState()
{
	Super::UpdateState();

	if (OwnerCharacter->MovementComponent->MovementMode == MOVE_Walking)
	{
		ChangeState(EPlayerStateType::Idle);
	}
}
