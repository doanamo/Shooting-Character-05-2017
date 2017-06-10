// Copyright (c) 2017 Piotr Doan. All Rights Reserved.

#include "Game.h"
#include "CharacterBase.h"
#include "CharacterBaseAnimation.h"

#include <GameFramework/PlayerController.h>
#include "Gameplay/Health/HealthComponent.h"
#include "Weapons/WeaponBase.h"
#include "Items/ItemBase.h"

ACharacterBase::ACharacterBase() :
	Health(nullptr),
	SkeletalMesh(nullptr),
	AnimationInstance(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;

	// Set default movement speed.
	MaxWalkSpeed = 140.0f;
	MaxJogSpeed = 280.0f;

	GetCharacterMovement()->MaxWalkSpeed = MaxJogSpeed;

	// Do not update the controller's rotation yaw.
	// Has to be disabled for "Orient Rotation to Movement" to work.
	bUseControllerRotationYaw = false;

	// Rotate the character during movement by default.
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// Character interaction values.
	MaxPickUpDistance = 200.0f;

	// Character weapon handling values.
	CurrentWeapon = nullptr;

	bIsAiming = false;
	bIsFiring = false;
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

	// Check if we have a weapon.
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
			// Cast cursor trace to world.
			FVector MouseLocation;
			FVector MouseDirection;

			bool Success = PlayerController->DeprojectMousePositionToWorld(MouseLocation, MouseDirection);

			// Rotate the character towards the cursor.
			if(Success)
			{
				// Cast the cursor onto a plane.
				FVector LineBegin = MouseLocation;
				FVector LineEnd = MouseLocation + MouseDirection * 10000.0f;

				FVector PlaneOrigin = CurrentWeapon->GetMuzzleLocation();
				FVector PlaneNormal = FVector(0.0f, 0.0f, 1.0f);

				FVector LookLocation = FMath::LinePlaneIntersection(LineBegin, LineEnd, PlaneOrigin, PlaneNormal);

				// Rotate the cursor toward the intersection of the cursor and the plane.
				FRotator LookRotation = (LookLocation - GetActorLocation()).Rotation();

				LookRotation.Pitch = 0.0f;
				LookRotation.Roll = 0.0f;

				SetActorRotation(FMath::RInterpTo(GetActorRotation(), LookRotation, DeltaTime, 10.0f));
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

void ACharacterBase::PickUp(AItemBase* Item)
{
	// Drop the current weapon.
	if(CurrentWeapon != nullptr)
	{
		// Unsubscribe from weapon's events.
		CurrentWeapon->OnWeaponFired.RemoveDynamic(this, &ACharacterBase::OnWeaponFired);

		// Detach weapon from the character.
		CurrentWeapon->Detach();
		CurrentWeapon = nullptr;
	}

	// Return if null character has been passed.
	if(Item == nullptr)
		return;

	// Check distance from the object.
	float Distance = Item->GetDistanceTo(this);

	if(Distance > MaxPickUpDistance)
		return;

	// Check if the item is a weapon.
	AWeaponBase* Weapon = Cast<AWeaponBase>(Item);

	if(Weapon != nullptr)
	{
		// Attach weapon to the character.
		CurrentWeapon = Weapon;
		CurrentWeapon->Attach(this);

		// Subscribe to weapon's events.
		CurrentWeapon->OnWeaponFired.AddDynamic(this, &ACharacterBase::OnWeaponFired);
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

	// Stop ticking while dead.
	PrimaryActorTick.bCanEverTick = false;

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
