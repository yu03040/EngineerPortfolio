// Fill out your copyright notice in the Description page of Project Settings.


#include "Rifle.h"

// Sets default values
ARifle::ARifle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// SceneComponentを作成する
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));

	// SceneComponent を RootComponentに設定する
	RootComponent = DefaultSceneRoot;

	// SkeletalMeshComponen を作成する
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));

	// RootComponent の子に
	WeaponMesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ARifle::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARifle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

