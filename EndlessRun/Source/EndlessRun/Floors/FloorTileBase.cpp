// Fill out your copyright notice in the Description page of Project Settings.


#include "EndlessRun/Floors/FloorTileBase.h"
#include "EndlessRun/GameModes/RunGameMode.h"
#include "EndlessRun/Characters/RunCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AFloorTileBase::AFloorTileBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// SceneComponent を初期化
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = DefaultSceneRoot;

	// EndTrigger を初期化
	EndTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerCollisionComponent"));
	EndTrigger->SetupAttachment(RootComponent);
	// 通り過ぎた床は消していく
	EndTrigger->OnComponentBeginOverlap.AddDynamic(this, &AFloorTileBase::OnOverlapBegin);

	// CoinArea を初期化
	CoinArea = CreateDefaultSubobject<UBoxComponent>(TEXT("CoinCollisionComponent"));
	CoinArea->SetupAttachment(RootComponent);

	// SpawnPoint の初期化
	SpawnPointCenter = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	SpawnPointLeft = CreateDefaultSubobject<UArrowComponent>(TEXT("LeftArrowComponent"));
	SpawnPointRight = CreateDefaultSubobject<UArrowComponent>(TEXT("RightArrowComponent"));
	SpawnPointCenter->SetupAttachment(RootComponent);
	SpawnPointLeft->SetupAttachment(RootComponent);
	SpawnPointRight->SetupAttachment(RootComponent);

	// AttachPoint の初期化
	AttachPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("AttachArrowComponent"));
	AttachPoint->SetupAttachment(RootComponent);
	AttachPoint->SetRelativeLocation(FVector(500.0f, 0.0f, 0.0f));
}

// Called when the game starts or when spawned
void AFloorTileBase::BeginPlay()
{
	Super::BeginPlay();

}

void AFloorTileBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto Player = Cast<ARunCharacter>(OtherActor);
	if (Player)
	{
		auto GameMode = Cast<ARunGameMode>(UGameplayStatics::GetGameMode(this));
		if (GameMode)
		{
			GameMode->AddFloorTile();

			float DelayTime = 2.0f;

			FLatentActionInfo LatentActionInfo;
			LatentActionInfo.CallbackTarget = this;
			LatentActionInfo.ExecutionFunction = FName(TEXT("Destroy"));
			LatentActionInfo.Linkage = 0;

			// 通り過ぎた床は 2 秒後に消していく
			UKismetSystemLibrary::Delay(this, DelayTime, LatentActionInfo);
		}
	}
}

// Called every frame
void AFloorTileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FTransform AFloorTileBase::GetAttachPoint()
{
	return AttachPoint->GetComponentTransform();
}

void AFloorTileBase::SetSpawnPoint_Implementation()
{
	FTransform R = SpawnPointRight->GetRelativeTransform();
	FTransform C = SpawnPointCenter->GetRelativeTransform();
	FTransform L = SpawnPointLeft->GetRelativeTransform();
	SpawnPoints.Add(R);
	SpawnPoints.Add(C);
	SpawnPoints.Add(L);

	// コインや障害物をランダムに生成する
	switch (static_cast<SpawnObj>(UKismetMathLibrary::RandomIntegerInRange(0, 1)))
	{
	case SpawnObj::Blocker:
		SpawnBlocker();
		break;

	case SpawnObj::Coins:
		SpawnCoins();
		break;
	}
}

void AFloorTileBase::SpawnBlocker_Implementation()
{
}

void AFloorTileBase::SpawnCoins_Implementation()
{
}


