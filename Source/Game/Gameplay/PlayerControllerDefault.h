// Copyright (c) 2017 Piotr Doan. All Rights Reserved.

#pragma once

#include <GameFramework/PlayerController.h>
#include "PlayerControllerDefault.generated.h"

UCLASS()
class GAME_API APlayerControllerDefault : public APlayerController
{
	GENERATED_BODY()
	
protected:
	APlayerControllerDefault();

	virtual void BeginPlay() override;
};
