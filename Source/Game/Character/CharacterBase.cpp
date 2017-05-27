// Copyright (c) 2017 Piotr Doan. All Rights Reserved.

#include "Game.h"
#include "CharacterBase.h"
#include "CharacterBaseAnimation.h"
#include "PlayerControllerDefault.h"

ACharacterBase::ACharacterBase() :
	AnimationInstance(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;

	// Character movement speed values.
	MaxWalkSpeed = 160.0f;
	MaxJogSpeed = 280.0f;

	// Set default movement speed.
	GetCharacterMovement()->MaxWalkSpeed = MaxJogSpeed;

	// Do not update the controller's rotation yaw.
	// Has to be disabled for "Orient Rotation to Movement" to work.
	bUseControllerRotationYaw = false;

	// Rotate the character during movement.
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void ACharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Retrieve the skeletal mesh.
	USkeletalMeshComponent* SkeletalMesh = GetMesh();
	check(SkeletalMesh != nullptr && "Character does not have a skeletal mesh!");

	// Retrieve the animation instance.
	AnimationInstance = Cast<UCharacterBaseAnimation>(SkeletalMesh->GetAnimInstance());
	check(AnimationInstance != nullptr && "Character does not have an animation instance!");
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
	AnimationInstance->bIsMoving = Velocity > 1.0f;
	AnimationInstance->MovementSpeed = Velocity;

	// Calculate strafing rotation.
	FVector MovementDirection = GetLastMovementInputVector();
	FVector CharacterDirection = GetActorForwardVector();

	float StrafingRotation = FMath::Atan2(MovementDirection.Y, MovementDirection.X) - FMath::Atan2(CharacterDirection.Y, CharacterDirection.X);

	if(FMath::Abs(StrafingRotation) > PI)
	{
		StrafingRotation = StrafingRotation > 0 ? StrafingRotation - PI * 2.0f : StrafingRotation + PI * 2.0f;
	}

	StrafingRotation = FMath::RadiansToDegrees(StrafingRotation);

	AnimationInstance->StrafingRotation = StrafingRotation;

	// Rotate the character toward the aiming point.
	if(AnimationInstance->bIsAiming)
	{
		auto PlayerController = Cast<APlayerControllerDefault>(GetController());

		if(PlayerController)
		{
			FVector MousePosition;
			FVector MouseDirection;

			if(PlayerController->DeprojectMousePositionToWorld(MousePosition, MouseDirection))
			{
				FVector LookLocation = FMath::LinePlaneIntersection(MousePosition, MousePosition + MouseDirection * 10000.0f, FVector(0.0f, 0.0f, 0.0f), FVector(0.0f, 0.0f, 1.0f));

				FRotator LookRotation = (LookLocation - GetActorLocation()).Rotation();
				LookRotation.Pitch = 0.0f;
				LookRotation.Roll = 0.0f;

				SetActorRotation(FMath::Lerp(GetActorRotation(), LookRotation, 0.05f));
			}
		}
	}
}

void ACharacterBase::AimPressed()
{
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;

	AnimationInstance->bIsAiming = true;
}

void ACharacterBase::AimReleased()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = MaxJogSpeed;

	AnimationInstance->bIsAiming = false;
}

void ACharacterBase::MoveVertical(float Value)
{
	AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value);
}

void ACharacterBase::MoveHorizontal(float Value)
{
	AddMovementInput(FVector(0.0f, 1.0f, 0.0f), Value);
}
