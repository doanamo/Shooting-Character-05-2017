// Copyright (c) 2017 Piotr Doan. All Rights Reserved.

#pragma once

#include <GameFramework/Character.h>
#include "CharacterBase.generated.h"

UCLASS()
class GAME_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	float MaxWalkSpeed;

	UPROPERTY(EditDefaultsOnly)
	float MaxJogSpeed;

	UPROPERTY(EditDefaultsOnly)
	float MaxPickUpDistance;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* FireHipAnimation;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* FireAimAnimation;

public:
	void Move(FVector Direction, float Scale);
	void Fire(bool Toggle);
	void Aim(bool Toggle);

	void PickUp(AActor* Actor);

protected:
	ACharacterBase();

	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	class USkeletalMeshComponent* SkeletalMesh;
	class UCharacterBaseAnimation* AnimationInstance;

	UPROPERTY()
	class AWeaponBase* CurrentWeapon;

	bool bIsAiming;
	bool bIsFiring;
};
