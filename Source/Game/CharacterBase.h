// Copyright (c) 2017 Piotr Doan. All Rights Reserved.

#pragma once

#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

UCLASS()
class GAME_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ACharacterBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
