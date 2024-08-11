// Fill out your copyright notice in the Description page of Project Settings.


#include "EndlessRun/GameModes/RunGameMode.h"
#include "EndlessRun/Characters/RunCharacter.h"
#include "EndlessRun/Floors/FloorTileBase.h"
#include "EndlessRun/UMG/ResultMenu/ResultMenu.h"
#include "EndlessRun/UMG/ResultMenu/Ranking.h"
#include "EndlessRun/UMG/GamePlaying/RunScore.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Sound/SoundBase.h"

ARunGameMode::ARunGameMode()
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;

	NextAttachPoint = UKismetMathLibrary::MakeTransform(
		FVector(0.0f, 0.0f, 0.0f),
		FRotator(0.0f, 0.0f, 0.0f),
		FVector(1.0f, 1.0f, 1.0f)
	);
	CurrentStraights = 0;
	MoveSeconds = 0.0f;
	SumDistance = 0.0f;
	TotalCoins = 0;
	SaveSlotName = TEXT("slot_0");
	SaveUserIndex = 0;
}

void ARunGameMode::BeginPlay()
{
	Super::BeginPlay();

	LoadGame();

	FString Path = TEXT("/Game/EndlessRun/Blueprints/UMG/WBP_RunScore_v2.WBP_RunScore_v2_C");
	TSubclassOf<UUserWidget> WidgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*Path)).LoadSynchronous();

	TObjectPtr<APlayerController> PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (WidgetClass && PlayerController)
	{
		TObjectPtr<URunScore> RunScoreWidget = Cast<URunScore>(UWidgetBlueprintLibrary::Create(this, WidgetClass, PlayerController));
		RunScoreWidget->AddToViewport();
	}

	TObjectPtr<AActor> Floor = GetWorld()->SpawnActor<AActor>(FloorTiles[0], NextAttachPoint);
	TObjectPtr<AFloorTileBase> FloorBase = Cast<AFloorTileBase>(Floor);
	if (FloorBase)
	{
		NextAttachPoint = FloorBase->GetAttachPoint();
		UpdateStraightsFloor();
	}

	for (int i = 0; i < 10; i++)
	{
		AddFloorTile();
	}
}

void ARunGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TObjectPtr<ARunCharacter> Player = Cast<ARunCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (Player->bIsDead == false)
	{
		RunDistance(DeltaTime, Player);
	}
}

void ARunGameMode::AddFloorTile()
{
	if (CurrentStraights > 7)
	{
		AddFloorTileCurve();
	}
	else
	{
		int LastIndex = FloorTiles.Num() - 1;
		int Min = 0;
		int Max = LastIndex;
		int CurrentIndex = UKismetMathLibrary::RandomIntegerInRange(Min, Max);
		TObjectPtr<AActor> Floor = GetWorld()->SpawnActor<AActor>(FloorTiles[CurrentIndex], NextAttachPoint);

		TObjectPtr<AFloorTileBase> FloorBase = Cast<AFloorTileBase>(Floor);
		if (FloorBase)
		{
			NextAttachPoint = FloorBase->GetAttachPoint();
			UpdateStraightsFloor();
		}
	}
}

void ARunGameMode::AddFloorTileCurve()
{
	int LastIndex = FloorCurves.Num() - 1;
	int Min = 0;
	int Max = LastIndex;
	int CurrentIndex = UKismetMathLibrary::RandomIntegerInRange(Min, Max);
	TObjectPtr<AActor> Floor = GetWorld()->SpawnActor<AActor>(FloorCurves[CurrentIndex], NextAttachPoint);

	TObjectPtr<AFloorTileBase> FloorBase = Cast<AFloorTileBase>(Floor);
	if (FloorBase)
	{
		NextAttachPoint = FloorBase->GetAttachPoint();
		InitStraightsFloor();
	}
}

void ARunGameMode::AddCoins()
{
	TotalCoins += 1;
	RankingData.Score = TotalCoins;
}

void ARunGameMode::RunDistance(float DeltaSeconds, TObjectPtr<ARunCharacter>& Player)
{
	// 1 フレーム = 1/30[s] だから 秒に変換して足す
	MoveSeconds += DeltaSeconds * 30.0f;

	// xy 方向の走った距離を求める
	SumDistance = UKismetMathLibrary::VSizeXY(Player->GetActorForwardVector() * MoveSeconds);
	RankingData.Distance = SumDistance;
}

void ARunGameMode::UpdateStraightsFloor()
{
	CurrentStraights += 1;
}

void ARunGameMode::InitStraightsFloor()
{
	CurrentStraights = 0;
}

void ARunGameMode::ShowResult()
{
	SetActorTickEnabled(false);

	FString Path = TEXT("/Game/EndlessRun/Blueprints/UMG/WBP_Result_v2.WBP_Result_v2_C");
	TSubclassOf<UUserWidget> WidgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*Path)).LoadSynchronous();

	TObjectPtr<APlayerController> PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (WidgetClass && PlayerController)
	{
		TObjectPtr<UResultMenu> ResultMenuWidget = Cast<UResultMenu>(UWidgetBlueprintLibrary::Create(this, WidgetClass, PlayerController));
		ResultMenuWidget->RankingData.Score = RankingData.Score;
		ResultMenuWidget->RankingData.Distance = RankingData.Distance;
		ResultMenuWidget->RankingDataAll = LoadRankingData;
		ResultMenuWidget->AddToViewport();
	}
}

void ARunGameMode::CreateSaveFile()
{
	// Save Game オブジェクトを作成し、デフォルトのスロットに保存する
	TObjectPtr<USaveGameBase> DataToSave = Cast<USaveGameBase>(UGameplayStatics::CreateSaveGameObject(USaveGameBase::StaticClass()));
	UGameplayStatics::SaveGameToSlot(DataToSave, SaveSlotName, SaveUserIndex);
}

void ARunGameMode::SaveGame()
{
	// 保存するデータの初期化
	TObjectPtr<USaveGameBase> DataToSave = Cast<USaveGameBase>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, SaveUserIndex));

	// セーブに使用する有効な Save Game オブジェクトがある場合
	if (DataToSave != nullptr)
	{
		DataToSave->RankingDataList = LoadRankingData;
		UGameplayStatics::SaveGameToSlot(DataToSave, SaveSlotName, SaveUserIndex);
	}
	else if (!UGameplayStatics::DoesSaveGameExist(SaveSlotName, SaveUserIndex))
	{
		// デフォルトの保存ファイルを作成する
		CreateSaveFile();
	}
}

void ARunGameMode::LoadGame()
{
	// ロードするデータを取得する
	TObjectPtr<USaveGameBase> DataToLoad = Cast<USaveGameBase>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, SaveUserIndex));

	// ロードするデータがある場合
	if (DataToLoad != nullptr)
	{
		LoadRankingData = DataToLoad->RankingDataList;
	}
	else if (!UGameplayStatics::DoesSaveGameExist(SaveSlotName, SaveUserIndex))
	{
		// デフォルトの保存ファイルを作成する
		CreateSaveFile();
	}
}
