// Copyright (c) 2017 Piotr Doan. All Rights Reserved.

#pragma once

#include <GameFramework/Actor.h>
#include "ProjectileBase.generated.h"

UCLASS()
class GAME_API AProjectileBase : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	float Damage;

	UPROPERTY(EditDefaultsOnly)
	float Speed;

	UPROPERTY(EditDefaultsOnly)
	float LifeTime;

protected:
	AProjectileBase();

	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnActorHit(AActor* Self, AActor* Other, FVector NormalImpulse, const FHitResult& Hit);

private:
	void DestroySelf();

private:
	FTimerHandle Timer;
};
