// Copyright (c) 2017 Piotr Doan. All Rights Reserved.

#include "Game.h"
#include "HealthComponent.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;

	// Set default values.
	MaximumHealth = 100.0f;
	CurrentHealth = -1.0f;
}

void UHealthComponent::InitializeComponent()
{
	Super::InitializeComponent();

	// Set current health to maximum health.
	if(CurrentHealth < 0.0f)
	{
		CurrentHealth = MaximumHealth;
	}

	// Subscribe to the delegate in the attached actor.
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnTakeAnyDamage);
}

void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	// Update health value.
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.0f, MaximumHealth);

	// Handle health value reaching zero.
	if(CurrentHealth <= 0.0f)
	{
		GetOwner()->Destroy();
	}
}
