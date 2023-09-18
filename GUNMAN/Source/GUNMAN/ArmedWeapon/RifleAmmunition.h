// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RifleAmmunition.generated.h"

UCLASS()
class GUNMAN_API ARifleAmmunition : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARifleAmmunition();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	/** シーンコンポーネント */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USceneComponent* DefaultSceneRoot;

	/** 弾薬のメッシュ */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMesh;
};
