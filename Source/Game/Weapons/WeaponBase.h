// Copyright (c) 2017 Piotr Doan. All Rights Reserved.

#pragma once

#include <GameFramework/Actor.h>
#include "WeaponBase.generated.h"

UCLASS()
class GAME_API AWeaponBase : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	float FireDelay;
	
public:	
	void Attach(class USkeletalMeshComponent* Mesh);
	void Detach();

	bool Fire(float DeltaTime);

protected:
	AWeaponBase();

	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	class UPrimitiveComponent* Primitive;

	float FireTimer;
};
