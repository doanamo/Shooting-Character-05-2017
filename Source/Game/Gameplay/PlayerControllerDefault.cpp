// Copyright (c) 2017 Piotr Doan. All Rights Reserved.

#include "Game.h"
#include "PlayerControllerDefault.h"
#include <Characters/CharacterBase.h>

APlayerControllerDefault::APlayerControllerDefault()
{
	bShowMouseCursor = true;
}

void APlayerControllerDefault::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveVertical", this, &APlayerControllerDefault::MoveVertical);
	InputComponent->BindAxis("MoveHorizontal", this, &APlayerControllerDefault::MoveHorizontal);

	InputComponent->BindAction("Fire", IE_Pressed, this, &APlayerControllerDefault::FirePressed);
	InputComponent->BindAction("Fire", IE_Released, this, &APlayerControllerDefault::FireReleased);

	InputComponent->BindAction("Aim", IE_Pressed, this, &APlayerControllerDefault::AimPressed);
	InputComponent->BindAction("Aim", IE_Released, this, &APlayerControllerDefault::AimReleased);

	InputComponent->BindAction("PickUp", IE_Pressed, this, &APlayerControllerDefault::PickUpPressed);
}

void APlayerControllerDefault::AcknowledgePossession(APawn* PossesedPawn)
{
	Super::AcknowledgePossession(PossesedPawn);

	Character = Cast<ACharacterBase>(PossesedPawn);
	check(Character != nullptr && "Possesed a pawn that isn't of a desired character type!");
}

void APlayerControllerDefault::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerControllerDefault::MoveVertical(float Scale)
{
	if(Character)
	{
		Character->Move(FVector(1.0f, 0.0f, 0.0f), Scale);
	}
}

void APlayerControllerDefault::MoveHorizontal(float Scale)
{
	if(Character)
	{
		Character->Move(FVector(0.0f, 1.0f, 0.0f), Scale);
	}
}

void APlayerControllerDefault::FirePressed()
{
	if(Character)
	{
		Character->Fire(true);
	}
}

void APlayerControllerDefault::FireReleased()
{
	if(Character)
	{
		Character->Fire(false);
	}
}

void APlayerControllerDefault::AimPressed()
{
	if(Character)
	{
		Character->Aim(true);
	}
}

void APlayerControllerDefault::AimReleased()
{
	if(Character)
	{
		Character->Aim(false);
	}
}

void APlayerControllerDefault::PickUpPressed()
{
	// TODO
}
