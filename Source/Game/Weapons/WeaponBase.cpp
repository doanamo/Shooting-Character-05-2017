// Copyright (c) 2017 Piotr Doan. All Rights Reserved.

#include "Game.h"
#include "WeaponBase.h"

AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
