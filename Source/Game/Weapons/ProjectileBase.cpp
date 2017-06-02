// Copyright (c) 2017 Piotr Doan. All Rights Reserved.

#include "Game.h"
#include "ProjectileBase.h"

AProjectileBase::AProjectileBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// Default projectile parameters.
	Damage = 10.0f;
	Speed = 1200.0f;
	LifeTime = 1.0f;
}

void AProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Subscribe to actor's hit event.
	AActor::OnActorHit.AddDynamic(this, &AProjectileBase::OnActorHit);
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

void AProjectileBase::OnActorHit(AActor* Self, AActor* Other, FVector NormalImpulse, const FHitResult& Hit)
{
	// Cause damage to the hit actor.
	if(Other != nullptr)
	{
		FDamageEvent DamageEvent;
		Other->TakeDamage(Damage, DamageEvent, nullptr, this);
	}

	// Destroy self.
	Destroy();
}

void AProjectileBase::DestroySelf()
{
	// Destroy self.
	Destroy();
}
