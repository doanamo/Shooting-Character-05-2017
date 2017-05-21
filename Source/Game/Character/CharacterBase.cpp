// Copyright (c) 2017 Piotr Doan. All Rights Reserved.

#include "Game.h"
#include "CharacterBase.h"

ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// Don't update the controller's rotation yaw.
	// Has to be disabled for "Orient Rotation to Movement" to work.
	bUseControllerRotationYaw = false;

	// Rotate the character during movement.
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Setup character input bindings.
	PlayerInputComponent->BindAxis("MoveVertical", this, &ACharacterBase::MoveVertical);
	PlayerInputComponent->BindAxis("MoveHorizontal", this, &ACharacterBase::MoveHorizontal);
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACharacterBase::MoveVertical(float Value)
{
	AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value);
}

void ACharacterBase::MoveHorizontal(float Value)
{
	AddMovementInput(FVector(0.0f, 1.0f, 0.0f), Value);
}
