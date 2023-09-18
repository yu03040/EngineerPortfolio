// Fill out your copyright notice in the Description page of Project Settings.

#include "CPP_test.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"

// Sets default values
ACPP_test::ACPP_test()
{
	// tick イベントは実行しない
	PrimaryActorTick.bCanEverTick = false;

	// StaticMesh コンポーネントを生成
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));

	// メッシュデータの読み込み
	ConstructorHelpers::FObjectFinder<UStaticMesh> staticMeshAsset(TEXT("/Engine/BasicShapes/Sphere.Sphere"));

	//メッシュデータの読み込みが成功したら mesh にメッシュを設定
	if (staticMeshAsset.Succeeded())
	{
		mesh->SetStaticMesh(staticMeshAsset.Object);

		// 物理シュミレーションをオンにする
		mesh->SetSimulatePhysics(true);
	}
}

// Called when the game starts or when spawned
void ACPP_test::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACPP_test::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

