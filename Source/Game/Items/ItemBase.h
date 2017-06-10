// Copyright (c) 2017 Piotr Doan. All Rights Reserved.

#pragma once

#include <GameFramework/Actor.h>
#include "ItemBase.generated.h"

UCLASS()
class GAME_API AItemBase : public AActor
{
	GENERATED_BODY()
	
protected:
	AItemBase();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:

};
