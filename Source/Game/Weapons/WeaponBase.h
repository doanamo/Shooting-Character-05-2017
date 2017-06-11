// Copyright (c) 2017 Piotr Doan. All Rights Reserved.

#pragma once

#include <GameFramework/Actor.h>
#include "Items/ItemBase.h"
#include "WeaponBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponFiredSignature);

UCLASS()
class GAME_API AWeaponBase : public AItemBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	float FireRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int MaximumAmmo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int CurrentAmmo;

	UPROPERTY(EditDefaultsOnly)
	class UClass* ProjectileType;

	UPROPERTY(BlueprintAssignable)
	FOnWeaponFiredSignature OnWeaponFired;
	
public:
	void Attach(class ACharacterBase* Character);
	void Detach();

	void PullTrigger();
	void ReleaseTrigger();

	FVector GetMuzzleLocation() const;

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
