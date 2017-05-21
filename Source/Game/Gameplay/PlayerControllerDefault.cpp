// Copyright (c) 2017 Piotr Doan. All Rights Reserved.

#include "Game.h"
#include "PlayerControllerDefault.h"

APlayerControllerDefault::APlayerControllerDefault()
{
	bShowMouseCursor = true;
}

void APlayerControllerDefault::BeginPlay()
{
	Super::BeginPlay();
}
