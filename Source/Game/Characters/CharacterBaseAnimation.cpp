// Copyright (c) 2017 Piotr Doan. All Rights Reserved.

#include "Game.h"
#include "CharacterBaseAnimation.h"

UCharacterBaseAnimation::UCharacterBaseAnimation() :
	bHasWeapon(false),
	bIsAiming(false),
	bIsMoving(false),
	MovementSpeed(0.0f),
	StrafingRotation(0.0f),
	OwningPawn(nullptr)
{
}

void UCharacterBaseAnimation::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwningPawn = TryGetPawnOwner();
}

void UCharacterBaseAnimation::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
}
