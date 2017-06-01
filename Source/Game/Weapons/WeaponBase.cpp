// Copyright (c) 2017 Piotr Doan. All Rights Reserved.

#include "Game.h"
#include "WeaponBase.h"
#include "Characters/CharacterBase.h"

#include <Components/SkeletalMeshComponent.h>
#include <Components/PrimitiveComponent.h>
#include <Components/ArrowComponent.h>

AWeaponBase::AWeaponBase() :
	Primitive(nullptr),
	Muzzle(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;

	// Set default variables.
	FireRate = 10.0f;
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
	// Start the firing timer.
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &AWeaponBase::Fire, 1.0f / FireRate, true);
}

void AWeaponBase::ReleaseTrigger()
{
	// Clear the firing timer.
	GetWorld()->GetTimerManager().ClearTimer(Timer);
}

void AWeaponBase::Fire()
{
	// Spawn a projectile.
	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = Cast<APawn>(GetOwner());

	FTransform Transform = Muzzle->GetComponentToWorld();
	GetWorld()->SpawnActor<AActor>(ProjectileType, Transform.GetLocation(), Transform.GetRotation().Rotator(), SpawnParams);

	// Broadcast a weapon fired event.
	OnWeaponFired.Broadcast();
}
