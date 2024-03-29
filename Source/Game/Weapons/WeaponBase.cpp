// Copyright (c) 2017 Piotr Doan. All Rights Reserved.

#include "Game.h"
#include "WeaponBase.h"

#include <Components/SkeletalMeshComponent.h>
#include <Components/PrimitiveComponent.h>
#include <Components/ArrowComponent.h>
#include "Characters/CharacterBase.h"

AWeaponBase::AWeaponBase() :
	Primitive(nullptr),
	Muzzle(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;

	// Set default variables.
	FireRate = 10.0f;

	MaximumAmmo = 30;
	CurrentAmmo = -1;
}

void AWeaponBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Get the root primitive component.
	Primitive = Cast<UPrimitiveComponent>(GetRootComponent());
	check(Primitive != nullptr && "Actor is mising a primitive component!");

	// Get the muzzle arrow component.
	auto Components = GetComponents();
	for(auto Component : Components)
	{
		if(Component->GetFName() == "Muzzle")
		{
			Muzzle = Cast<UArrowComponent>(Component);
			break;
		}
	}

	check(Muzzle != nullptr && "Actor is mising an arrow component for muzzle!");

	// Sanitize default values.
	MaximumAmmo = FMath::Max(1, MaximumAmmo);
	CurrentAmmo = FMath::Min(CurrentAmmo, MaximumAmmo);

	// Set current ammo to maximum ammo.
	if(CurrentAmmo < 0)
	{
		CurrentAmmo = MaximumAmmo;
	}
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeaponBase::Attach(class ACharacterBase* Character)
{
	verify(Character != nullptr && "Weapon's attach function called with null character!");

	// Set owner of this weapon.
	SetOwner(Character);

	// Disable weapon's physics.
	SetActorEnableCollision(false);
	Primitive->SetSimulatePhysics(false);

	// Attach weapon to the character's mesh.
	AttachToComponent(Character->GetSkeletalMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "WeaponSocket");
}

void AWeaponBase::Detach()
{
	// Stop firing when the weapon is detached.
	ReleaseTrigger();

	// Unset owner of the weapon.
	SetOwner(nullptr);

	// Re-enable weapon's physics.
	SetActorEnableCollision(true);
	Primitive->SetSimulatePhysics(true);

	// Detach weapon from the character's mesh.
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

void AWeaponBase::PullTrigger()
{
	auto& TimerManager = GetWorld()->GetTimerManager();

	// Check current ammo value before attempting to firing.
	if(CurrentAmmo > 0)
	{
		// Start the firing timer and use the remaining time of the previous timer.
		float RemainingTime = FMath::Max(TimerManager.GetTimerRemaining(FireTimer), 0.0f);
		TimerManager.SetTimer(FireTimer, this, &AWeaponBase::Fire, 1.0f / FireRate, true, RemainingTime);
	}
}

void AWeaponBase::ReleaseTrigger()
{
	auto& TimerManager = GetWorld()->GetTimerManager();

	// Replace timer with one that will prevent the primary fire timer from triggering again too quickly.
	if(TimerManager.TimerExists(FireTimer))
	{
		float RemainingTime = TimerManager.GetTimerRemaining(FireTimer);
		TimerManager.SetTimer(FireTimer, this, &AWeaponBase::ClearFireTimer, RemainingTime, false);
	}
}

void AWeaponBase::Fire()
{
	// Check current ammo value before actually firing.
	if(CurrentAmmo > 0)
	{
		// Spawn a projectile.
		FActorSpawnParameters SpawnParams;
		SpawnParams.Instigator = Cast<APawn>(GetOwner());

		FTransform Transform = Muzzle->GetComponentToWorld();
		GetWorld()->SpawnActor<AActor>(ProjectileType, Transform.GetLocation(), Transform.GetRotation().Rotator(), SpawnParams);

		// Broadcast a weapon fired event.
		OnWeaponFired.Broadcast();

		// Decrement the ammo count.
		CurrentAmmo -= 1;
	}
}

void AWeaponBase::ClearFireTimer()
{
	// Clear the timer after a delay set in ReleaseTrigger() function.
	GetWorld()->GetTimerManager().ClearTimer(FireTimer);
}

FVector AWeaponBase::GetMuzzleLocation() const
{
	return Muzzle->GetComponentToWorld().GetLocation();
}
