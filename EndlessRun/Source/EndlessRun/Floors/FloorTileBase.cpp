// Fill out your copyright notice in the Description page of Project Settings.


#include "EndlessRun/Floors/FloorTileBase.h"
#include "EndlessRun/GameModes/RunGameMode.h"
#include "EndlessRun/Characters/RunCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

AFloorTileBase::AFloorTileBase()
{
	// 毎フレームTick()を呼び出すように設定
	PrimaryActorTick.bCanEverTick = true;

	// SceneComponent を作成する
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = DefaultSceneRoot;

	// EndTrigger を作成する
	EndTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerCollisionComponent"));
	EndTrigger->SetupAttachment(RootComponent);

	// 通り過ぎた床は消していく
	EndTrigger->OnComponentBeginOverlap.AddDynamic(this, &AFloorTileBase::OnOverlapBegin);

	// CoinArea を作成する
	CoinArea = CreateDefaultSubobject<UBoxComponent>(TEXT("CoinCollisionComponent"));
	CoinArea->SetupAttachment(RootComponent);

	// SpawnPoint を作成する
	SpawnPointCenter = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	SpawnPointLeft = CreateDefaultSubobject<UArrowComponent>(TEXT("LeftArrowComponent"));
	SpawnPointRight = CreateDefaultSubobject<UArrowComponent>(TEXT("RightArrowComponent"));
	SpawnPointCenter->SetupAttachment(RootComponent);
	SpawnPointLeft->SetupAttachment(RootComponent);
	SpawnPointRight->SetupAttachment(RootComponent);

	// AttachPoint を作成する
	AttachPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("AttachArrowComponent"));
	AttachPoint->SetupAttachment(RootComponent);
	AttachPoint->SetRelativeLocation(FVector(500.0f, 0.0f, 0.0f));
}

void AFloorTileBase::BeginPlay()
{
	Super::BeginPlay();

}

void AFloorTileBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// EndTrigger に入ってきたのがプレイヤーだったら
	TObjectPtr<ARunCharacter> Player = Cast<ARunCharacter>(OtherActor);
	if (Player)
	{
		auto GameMode = Cast<ARunGameMode>(UGameplayStatics::GetGameMode(this));
		if (GameMode)
		{
			// 床を生成する
			GameMode->AddFloorTile();

			float DelayTime = 5.0f;

			FLatentActionInfo LatentActionInfo;
			LatentActionInfo.CallbackTarget = this;
			LatentActionInfo.ExecutionFunction = FName(TEXT("DestroyActor"));
			LatentActionInfo.Linkage = 0;

			// 通り過ぎた床は "DelayTime" 秒後に消していく
			UKismetSystemLibrary::Delay(this, DelayTime, LatentActionInfo);
		}
	}
}

void AFloorTileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFloorTileBase::DestroyActor()
{
	Destroy();
}

void AFloorTileBase::SetSpawnPoint_Implementation()
{
	// 障害物の位置を配列に入れる
	FTransform Right = SpawnPointRight->GetRelativeTransform();
	FTransform Center = SpawnPointCenter->GetRelativeTransform();
	FTransform Left = SpawnPointLeft->GetRelativeTransform();
	SpawnPoints.Add(Right);
	SpawnPoints.Add(Center);
	SpawnPoints.Add(Left);

	// コインや障害物をランダムに生成する
	switch (static_cast<ESpawnObj>(UKismetMathLibrary::RandomIntegerInRange(0, 1)))
	{
	case ESpawnObj::Blocker:
		SpawnBlocker();
		break;

	case ESpawnObj::Coins:
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


