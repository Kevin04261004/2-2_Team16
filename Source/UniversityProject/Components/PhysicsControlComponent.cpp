// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PhysicsControlComponent.h"

UPhysicsControlComponent::UPhysicsControlComponent()
{

}

void UPhysicsControlComponent::Initialize()
{
	CollisionComponent = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
}



void UPhysicsControlComponent::GoForward(float distance)
{
	// SimulatePhysics가 true이면, 캐릭터가 인풋으로 이동이 불가능함. 그래서, 0.2초정도 활성화 후 되돌리기.
	CollisionComponent->SetSimulatePhysics(true);
	CollisionComponent->SetEnableGravity(false);
	FVector impulseDirection = GetOwner()->GetActorForwardVector() * distance;
	impulseDirection.Z = 0;
	CollisionComponent->AddImpulse(impulseDirection * distance,"", true);
	GetWorld()->GetTimerManager().SetTimer(PhysicsTimerHandle, this, &UPhysicsControlComponent::SetPhysicsFalse, 0.2f, false);
}

void UPhysicsControlComponent::GoUp(float amount)
{
	// SimulatePhysics가 true이면, 캐릭터가 인풋으로 이동이 불가능함. 그래서, 0.2초정도 활성화 후 되돌리기.
	CollisionComponent->SetSimulatePhysics(true);
	CollisionComponent->SetEnableGravity(false);
	CollisionComponent->AddImpulse(FVector::DownVector * amount,"", true);
	GetWorld()->GetTimerManager().SetTimer(PhysicsTimerHandle, this, &UPhysicsControlComponent::SetPhysicsFalse, 0.2f, false);
}

void UPhysicsControlComponent::CollisionOff()
{
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UPhysicsControlComponent::CollisionOn()
{
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void UPhysicsControlComponent::SetPhysicsFalse()
{
	if (CollisionComponent != nullptr)
	{
		CollisionComponent->SetSimulatePhysics(false);
		CollisionComponent->SetEnableGravity(true);
	}
}
