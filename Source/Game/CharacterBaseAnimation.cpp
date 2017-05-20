// Copyright (c) 2017 Piotr Doan. All Rights Reserved.

#include "Game.h"
#include "CharacterBaseAnimation.h"

UCharacterBaseAnimation::UCharacterBaseAnimation() :
	bIsMoving(false)
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

	if(OwningPawn)
	{
		float Velocity = OwningPawn->GetVelocity().Size();

		bIsMoving = Velocity > 1.0f;
		MovementSpeed = Velocity;
	}
}
