// Copyright (c) 2017 Piotr Doan. All Rights Reserved.

#pragma once

#include <GameFramework/Actor.h>
#include "WeaponBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponFiredSignature);

UCLASS()
class GAME_API AWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	float FireRate;

	UPROPERTY(EditDefaultsOnly)
	class UClass* ProjectileType;

	UPROPERTY(BlueprintAssignable)
	FOnWeaponFiredSignature OnWeaponFired;
	
public:	
	void Attach(class ACharacterBase* Character);
	void Detach();

	void PullTrigger();
	void ReleaseTrigger();

protected:
	AWeaponBase();

	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void Fire();

private:
	void ClearFireTimer();
	FTimerHandle FireTimer;

private:
	class UPrimitiveComponent* Primitive;
	class UArrowComponent* Muzzle;
};
