// Fill out your copyright notice in the Description page of Project Settings.


#include "GUNMANAnimInstance.h"

void UGUNMANAnimInstance::AimingState_Implementation(bool bIsAiming)
{
	// 構えているか受け取る
	IsAiming = bIsAiming;
}

void UGUNMANAnimInstance::EquippedState_Implementation(bool bHasWeapon, bool bHasPistol)
{
	// 武器とピストルの状態を受け取る
	HasWeapon = bHasWeapon;
	HasPistol = bHasPistol;
}
