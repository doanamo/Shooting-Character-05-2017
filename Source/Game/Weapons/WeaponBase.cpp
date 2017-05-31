// Copyright (c) 2017 Piotr Doan. All Rights Reserved.

#include "Game.h"
#include "WeaponBase.h"

#include <Components/SkeletalMeshComponent.h>
#include <Components/PrimitiveComponent.h>

AWeaponBase::AWeaponBase() :
	Primitive(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;

	// Weapon handling values.
	FireDelay = 0.2f;
	FireTimer = 0.0f;
}

void AWeaponBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Primitive = Cast<UPrimitiveComponent>(GetRootComponent());
	check(Primitive != nullptr && "Actor is mising a primitive component!");
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeaponBase::Attach(USkeletalMeshComponent* Mesh)
{
	if(!Mesh)
		return;

	Primitive->SetSimulatePhysics(false);
	SetActorEnableCollision(false);
	AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetIncludingScale, "WeaponSocket");
}

void AWeaponBase::Detach()
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	SetActorEnableCollision(true);
	Primitive->SetSimulatePhysics(true);
}

bool AWeaponBase::Fire(float DeltaTime)
{
	// Handle weapon firing.
	FireTimer = FMath::Max(0.0f, FireTimer - DeltaTime);

	if(FireTimer == 0.0f)
	{
		//

		FireTimer = FireDelay;

		return true;
	}

	return false;
}
