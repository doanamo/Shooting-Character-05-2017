// Copyright (c) 2017 Piotr Doan. All Rights Reserved.

#include "Game.h"
#include "WeaponBase.h"

#include <Components/SkeletalMeshComponent.h>
#include <Components/PrimitiveComponent.h>
#include <Components/ArrowComponent.h>

AWeaponBase::AWeaponBase() :
	Primitive(nullptr),
	Muzzle(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;

	// Weapon handling values.
	FireDelay = 0.2f;
	FireTimer = 0.0f;
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

	// Update weapon's fire timer.
	FireTimer = FMath::Max(0.0f, FireTimer - DeltaTime);
}

void AWeaponBase::Attach(USkeletalMeshComponent* Mesh)
{
	if(!Mesh)
		return;

	// Attach weapon to the character's mesh.
	SetActorEnableCollision(false);
	Primitive->SetSimulatePhysics(false);
	AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetIncludingScale, "WeaponSocket");
}

void AWeaponBase::Detach()
{
	// Detach weapon from the character's mesh.
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	SetActorEnableCollision(true);
	Primitive->SetSimulatePhysics(true);
}

bool AWeaponBase::Fire(float DeltaTime)
{
	// Handle weapon firing.
	if(FireTimer == 0.0f)
	{
		// Spawn a projectile.
		FActorSpawnParameters SpawnParams;
		SpawnParams.Instigator = Cast<APawn>(GetOwner());

		FTransform Transform = Muzzle->GetComponentToWorld();
		GetWorld()->SpawnActor<AActor>(Projectile, Transform.GetLocation(), Transform.GetRotation().Rotator(), SpawnParams);

		// Increment the delay timer.
		FireTimer = FireDelay;

		return true;
	}

	return false;
}
