// Copyright (c) 2017 Piotr Doan. All Rights Reserved.

#include "Game.h"
#include "ProjectileBase.h"

AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// Default projectile parameters.
	Speed = 1200.0f;
	LifeTime = 1.0f;
}

void AProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	// Destroy the projectile after some time.
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &AProjectileBase::DestroySelf, LifeTime, false);
}

void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Move the projectile in a forward direction.
	FVector Location = GetActorLocation();
	Location += GetActorForwardVector() * Speed * DeltaTime;
	SetActorLocation(Location, true);
}

void AProjectileBase::DestroySelf()
{
	// Destroy self.
	Destroy();
}
