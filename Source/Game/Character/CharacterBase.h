// Copyright (c) 2017 Piotr Doan. All Rights Reserved.

#pragma once

#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

UCLASS()
class GAME_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

protected:
	ACharacterBase();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void AimPressed();
	void AimReleased();

	void MoveVertical(float Value);
	void MoveHorizontal(float Value);

private:
	class UCharacterBaseAnimation* AnimInstance;
};
