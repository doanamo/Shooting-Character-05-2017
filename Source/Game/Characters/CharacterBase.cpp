// Copyright (c) 2017 Piotr Doan. All Rights Reserved.

#include "Game.h"
#include "CharacterBase.h"
#include "CharacterBaseAnimation.h"
#include "Weapons/WeaponBase.h"
#include "Gameplay/Health/HealthComponent.h"
#include <GameFramework/PlayerController.h>

ACharacterBase::ACharacterBase() :
	Health(nullptr),
	SkeletalMesh(nullptr),
	AnimationInstance(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;

	// Character movement speed values.
	MaxWalkSpeed = 140.0f;
	MaxJogSpeed = 280.0f;

	// Character interaction values.
	MaxPickUpDistance = 200.0f;

	// Character weapon handling values.
	CurrentWeapon = nullptr;

	bIsAiming = false;
	bIsFiring = false;

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

	// Retrieve the health component.
	Health = FindComponentByClass<UHealthComponent>();
	check(Health != nullptr && "Character does not have a health component!");

	// Subscribe to health component's events.
	Health->OnDeath.AddDynamic(this, &ACharacterBase::OnDeath);

	// Retrieve the skeletal mesh component.
	SkeletalMesh = GetMesh();
	check(SkeletalMesh != nullptr && "Character does not have a skeletal mesh component!");

	// Retrieve the animation instance.
	AnimationInstance = Cast<UCharacterBaseAnimation>(SkeletalMesh->GetAnimInstance());
	check(AnimationInstance != nullptr && "Character does not have an animation instance!");
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const bool bHasWeapon = CurrentWeapon != nullptr;

	// Handle movement orientation and speed.
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = MaxJogSpeed;

	if(bHasWeapon && (bIsAiming || bIsFiring))
	{
		GetCharacterMovement()->bOrientRotationToMovement = false;

		if(bIsAiming)
		{
			GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
		}

		if(bIsFiring)
		{
			GetCharacterMovement()->MaxWalkSpeed *= 0.8;
		}
	}

	// Rotate the character towards the aiming point.
	if(bHasWeapon && (bIsAiming || bIsFiring))
	{
		auto PlayerController = Cast<APlayerController>(GetController());

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

	// Set animation weapon parameters.
	AnimationInstance->bHasWeapon = bHasWeapon;
	AnimationInstance->bIsAiming = bIsAiming;

	// Set animation movement parameters.
	float CurrentSpeed = GetVelocity().Size();
	bool bIsMoving = CurrentSpeed > 0.0f && GetCharacterMovement()->IsMovingOnGround();

	AnimationInstance->bIsMoving = bIsMoving;
	AnimationInstance->MovementSpeed = bIsMoving ? CurrentSpeed : 0.0f;

	// Set animation strafing rotation paremeter.
	FVector MovementDirection = GetLastMovementInputVector();
	FVector CharacterDirection = GetActorForwardVector();

	if(!MovementDirection.IsNearlyZero())
	{
		float StrafingRotation = FMath::Atan2(MovementDirection.Y, MovementDirection.X) - FMath::Atan2(CharacterDirection.Y, CharacterDirection.X);

		if(FMath::Abs(StrafingRotation) > PI)
		{
			StrafingRotation = StrafingRotation > 0 ? StrafingRotation - PI * 2.0f : StrafingRotation + PI * 2.0f;
		}

		StrafingRotation = FMath::RadiansToDegrees(StrafingRotation);

		AnimationInstance->StrafingRotation = StrafingRotation;
	}
}

void ACharacterBase::Move(FVector Direction, float Scale)
{
	AddMovementInput(Direction, Scale);
}

void ACharacterBase::Fire(bool Toggle)
{
	if(CurrentWeapon)
	{
		if(Toggle)
		{
			CurrentWeapon->PullTrigger();
		}
		else
		{
			CurrentWeapon->ReleaseTrigger();
		}
		
		bIsFiring = Toggle;
	}
}

void ACharacterBase::Aim(bool Toggle)
{
	bIsAiming = Toggle;
}

void ACharacterBase::PickUp(AActor* Actor)
{
	// Drop the current weapon.
	if(CurrentWeapon)
	{
		// Unsubscribe from weapon's events.
		CurrentWeapon->OnWeaponFired.RemoveDynamic(this, &ACharacterBase::OnWeaponFired);

		// Detach weapon from the character.
		CurrentWeapon->Detach();
		CurrentWeapon = nullptr;
	}

	// Return if null character has been passed.
	if(Actor == nullptr)
		return;

	// Check distance from the object.
	float Distance = Actor->GetDistanceTo(this);

	// Pick up the weapon.
	if(Distance <= MaxPickUpDistance)
	{
		AWeaponBase* Weapon = Cast<AWeaponBase>(Actor);

		if(Weapon)
		{
			// Attach weapon to the character.
			CurrentWeapon = Weapon;
			CurrentWeapon->Attach(this);

			// Subscribe to weapon's events.
			CurrentWeapon->OnWeaponFired.AddDynamic(this, &ACharacterBase::OnWeaponFired);
		}
	}
}

void ACharacterBase::OnWeaponFired()
{
	// Play recail animation depending on the stance.
	if(bIsAiming)
	{
		AnimationInstance->Montage_Play(FireAimAnimation);
	}
	else
	{
		AnimationInstance->Montage_Play(FireHipAnimation);
	}
}

void ACharacterBase::OnDeath()
{
	check(Health->IsDead() && "Called OnDeath() while alive!");

	// Drop held weapon.
	PickUp(nullptr);

	// Disable character's capsule collision.
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Allow character's ragdoll to be pushed around.
	/*
	SkeletalMesh->SetCollisionProfileName(TEXT("BlockAll"));
	SkeletalMesh->CanCharacterStepUpOn = ECB_No;
	SkeletalMesh->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	*/

	// Simulate character's ragdoll.
	SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SkeletalMesh->SetSimulatePhysics(true);
}

class USkeletalMeshComponent* ACharacterBase::GetSkeletalMesh()
{
	return SkeletalMesh;
}
