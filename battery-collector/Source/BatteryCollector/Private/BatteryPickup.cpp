// Fill out your copyright notice in the Description page of Project Settings.

#include "BatteryPickup.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"

ABatteryPickup::ABatteryPickup()
{
	GetMesh()->SetSimulatePhysics(true);
	SetPower(150.0f);
}

void ABatteryPickup::SetPower(float newPower)
{
	BatteryPower = newPower;
}

void ABatteryPickup::WasCollected_Implementation()
{
	Super::WasCollected_Implementation();
	UE_LOG(LogTemp, Warning, TEXT("C++ called"))

		Destroy();
}
