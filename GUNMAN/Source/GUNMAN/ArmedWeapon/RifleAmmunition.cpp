// Fill out your copyright notice in the Description page of Project Settings.


#include "../ArmedWeapon/RifleAmmunition.h"
#include "GameFramework/Character.h"
#include "Kismet/GamePlayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ARifleAmmunition::ARifleAmmunition()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// SceneComponent を作成する
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));

	// SceneComponent を RootComponent に設定する
	RootComponent = DefaultSceneRoot;

	// StaticMeshComponent を作成する
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

	// StaticMesh を Load して StaticMeshComponent の StaticMesh に設定する
	//FString RifleAmmunitionPath = TEXT("/Game/MilitaryWeapSilver/Weapons/AssaultRifleA_Ammo.AssaultRifleA_Ammo_C");
	//UStaticMesh* Mesh = LoadObject<UStaticMesh>(NULL, (*RifleAmmunitionPath), NULL, LOAD_None, NULL);
	//StaticMesh->SetStaticMesh(Mesh);

	// StaticMeshComponentをRootComponentにAttachする
	StaticMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ARifleAmmunition::BeginPlay()
{
	Super::BeginPlay();

	// プレイヤーをもらう
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	// アクターにキャスト
	AActor* MyActor = CastChecked<AActor>(Player);
	if (Player)
	{
		// 飛ばす力
		FVector Impulse = MyActor->GetActorRightVector() * UKismetMathLibrary::RandomFloatInRange(1.0f, 5.0f);
		// 物理シュミレーションをオン
		StaticMesh->SetSimulatePhysics(true);
		// Impulse 分弾薬を飛ばす
		StaticMesh->AddImpulse(Impulse);
	}
	
}

// Called every frame
void ARifleAmmunition::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

