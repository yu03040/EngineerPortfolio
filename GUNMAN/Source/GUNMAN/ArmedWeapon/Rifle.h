// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Rifle.generated.h"

UCLASS()
class GUNMAN_API ARifle : public AActor
{
	GENERATED_BODY()

		/** Scene Component */
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Rifle, meta = (AllowPrivateAccess = "true"))
		USceneComponent* DefaultSceneRoot;

	/** 武器のメッシュ */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Rifle, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* WeaponMesh;
public:
	// Sets default values for this actor's properties
	ARifle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
