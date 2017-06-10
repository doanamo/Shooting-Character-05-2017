// Copyright (c) 2017 Piotr Doan. All Rights Reserved.

#include "Game.h"
#include "ItemBase.h"

AItemBase::AItemBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AItemBase::BeginPlay()
{
	Super::BeginPlay();
}

void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
