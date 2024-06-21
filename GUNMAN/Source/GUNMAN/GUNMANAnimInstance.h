// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ArmedWeapon/AnimationInterface.h"
#include "GUNMANAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class GUNMAN_API UGUNMANAnimInstance : public UAnimInstance, public IAnimationInterface
{
	GENERATED_BODY()

protected:
	/* 武器を構えているか？ */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	bool IsAiming = false;

	/* 武器を持っているか？ */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	bool HasWeapon = false;

	/* ピストルを持っているか？ */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	bool HasPistol = false;

public:
	
	void AimingState_Implementation(bool bIsAiming) override;

	void EquippedState_Implementation(bool bHasWeapon, bool bHasPistol) override;

};
