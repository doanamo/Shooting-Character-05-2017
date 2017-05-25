// Copyright (c) 2017 Piotr Doan. All Rights Reserved.

#include "Game.h"
#include "CharacterBase.h"
#include "CharacterBaseAnimation.h"

ACharacterBase::ACharacterBase() :
	AnimInstance(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;

	// Do not update the controller's rotation yaw.
	// Has to be disabled for "Orient Rotation to Movement" to work.
	bUseControllerRotationYaw = false;

	// Rotate the character during movement.
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void ACharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Retrieve skeletal mesh.
	USkeletalMeshComponent* SkeletalMesh = GetMesh();
	check(SkeletalMesh != nullptr && "Character does not have a skeletal mesh!");

	// Retrieve animation instance.
	AnimInstance = Cast<UCharacterBaseAnimation>(SkeletalMesh->GetAnimInstance());
	check(AnimInstance != nullptr && "Character does not have an animation instance!");
}

void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Setup character input bindings.
	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &ACharacterBase::AimPressed);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &ACharacterBase::AimReleased);

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

	// Set animation movement parameters.
	float Velocity = GetVelocity().Size();
	AnimInstance->bIsMoving = Velocity > 1.0f;
	AnimInstance->MovementSpeed = Velocity;
}

void ACharacterBase::AimPressed()
{
	GetCharacterMovement()->bOrientRotationToMovement = false;
	AnimInstance->bIsAiming = true;
}

void ACharacterBase::AimReleased()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	AnimInstance->bIsAiming = false;
}

void ACharacterBase::MoveVertical(float Value)
{
	AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value);
}

void ACharacterBase::MoveHorizontal(float Value)
{
	AddMovementInput(FVector(0.0f, 1.0f, 0.0f), Value);
}
