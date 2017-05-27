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
	MaxWalkSpeed = 140.0f;
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
			FVector MouseLocation;
			FVector MouseDirection;

			if(PlayerController->DeprojectMousePositionToWorld(MouseLocation, MouseDirection))
			{
				FVector LineBegin = MouseLocation;
				FVector LineEnd = MouseLocation + MouseDirection * 10000.0f;

				FVector PlaneOrigin = GetActorLocation();
				FVector PlaneNormal = FVector(0.0f, 0.0f, 1.0f);

				FVector LookLocation = FMath::LinePlaneIntersection(LineBegin, LineEnd, PlaneOrigin, PlaneNormal);

				FRotator LookRotation = (LookLocation - GetActorLocation()).Rotation();
				LookRotation.Pitch = 0.0f;
				LookRotation.Roll = 0.0f;

				SetActorRotation(FMath::Lerp(GetActorRotation(), LookRotation, 0.05f));
			}
		}
	}

	// Set animation movement parameters.
	bool bIsMoving = GetCharacterMovement()->IsMovingOnGround();
	float CurrentSpeed = GetVelocity().Size();

	AnimationInstance->bIsMoving = bIsMoving;
	AnimationInstance->MovementSpeed = bIsMoving ? CurrentSpeed : 0.0f;
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
