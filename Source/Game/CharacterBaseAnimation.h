// Copyright (c) 2017 Piotr Doan. All Rights Reserved.

#pragma once

#include "Animation/AnimInstance.h"
#include "CharacterBaseAnimation.generated.h"

UCLASS()
class GAME_API UCharacterBaseAnimation : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsMoving;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float MovementSpeed;

protected:
	UCharacterBaseAnimation();

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

protected:
	APawn* OwningPawn;
};
