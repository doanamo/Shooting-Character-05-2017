// Copyright (c) 2017 Piotr Doan. All Rights Reserved.

#pragma once

#include <GameFramework/Actor.h>
#include "WeaponBase.generated.h"

UCLASS()
class GAME_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeaponBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
