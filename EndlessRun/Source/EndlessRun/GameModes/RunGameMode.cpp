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
	// 毎フレームTick()を呼び出すように設定
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

	// セーブデータをロードする、ない場合はセーブデータを作成する
	LoadGame();

	// ゲーム中の UI を表示する
	DisplayRunScore();

	// 最初にコインと障害物がない床を一つ生成する
	AddNothingSpawnFloorTile();

	// 最初プレイヤーが落ちないように 10 枚床を生成しておく
	for (int i = 0; i < 10; i++)
	{
		AddFloorTile();
	}
}

void ARunGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 走った距離を求める
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
		// 7 枚ごとに曲がった床を追加
		AddFloorTileCurve();
	}
	else
	{
		int LastIndex = FloorTiles.Num() - 1;
		int Min = 0;
		int Max = LastIndex;
		int CurrentIndex = UKismetMathLibrary::RandomIntegerInRange(Min, Max);

		TObjectPtr<AActor> SpawnActor = GetWorld()->SpawnActor<AActor>(FloorTiles[CurrentIndex], NextAttachPoint);

		TObjectPtr<AFloorTileBase> Floor = Cast<AFloorTileBase>(SpawnActor);
		if (Floor)
		{
			NextAttachPoint = Floor->GetAttachPoint();
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

	TObjectPtr<AActor> SpawnActor = GetWorld()->SpawnActor<AActor>(FloorCurves[CurrentIndex], NextAttachPoint);

	TObjectPtr<AFloorTileBase> Floor = Cast<AFloorTileBase>(SpawnActor);
	if (Floor)
	{
		NextAttachPoint = Floor->GetAttachPoint();
		InitStraightsFloor();
	}
}

void ARunGameMode::AddNothingSpawnFloorTile()
{
	int NotSpawningActorsFloorTileIndex = 0;
	TObjectPtr<AActor> SpawnActor = GetWorld()->SpawnActor<AActor>(FloorTiles[NotSpawningActorsFloorTileIndex], NextAttachPoint);
	
	TObjectPtr<AFloorTileBase> Floor = Cast<AFloorTileBase>(SpawnActor);
	if (Floor)
	{
		NextAttachPoint = Floor->GetAttachPoint();
		UpdateStraightsFloor();
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
	// Tick を止める
	SetActorTickEnabled(false);

	// WidgetBlueprint の Class を取得する
	FString Path = TEXT("/Game/EndlessRun/Blueprints/UMG/WBP_Result_v2.WBP_Result_v2_C");
	TSubclassOf<UUserWidget> WidgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*Path)).LoadSynchronous();

	// PlayerController を取得する
	TObjectPtr<APlayerController> PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	// WidgetClass と PlayerController が取得できたか判定する
	if (WidgetClass && PlayerController)
	{
		// リザルトメニュー用のウィジェットを作成する
		TObjectPtr<UResultMenu> ResultMenuWidget = Cast<UResultMenu>(UWidgetBlueprintLibrary::Create(this, WidgetClass, PlayerController));
		if (ResultMenuWidget)
		{
			// ランキングデータを更新
			ResultMenuWidget->RankingData.Score = RankingData.Score;
			ResultMenuWidget->RankingData.Distance = RankingData.Distance;
			ResultMenuWidget->RankingDataAll = RankingDataArray;

			// リザルトメニューを画面に表示する
			ResultMenuWidget->AddToViewport();
		}
	}
}

void ARunGameMode::DisplayRunScore()
{
	// WidgetBlueprint の Class を取得する
	FString Path = TEXT("/Game/EndlessRun/Blueprints/UMG/WBP_RunScore_v2.WBP_RunScore_v2_C");
	TSubclassOf<UUserWidget> WidgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*Path)).LoadSynchronous();

	// PlayerController を取得する
	TObjectPtr<APlayerController> PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	// WidgetClass と PlayerController が取得できたか判定する
	if (WidgetClass && PlayerController)
	{
		// スコア用のウィジェットを作成する
		TObjectPtr<URunScore> RunScoreWidget = Cast<URunScore>(UWidgetBlueprintLibrary::Create(this, WidgetClass, PlayerController));
		if (RunScoreWidget)
		{
			// スコアを画面に表示する
			RunScoreWidget->AddToViewport();
		}
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
		DataToSave->RankingDataArray = RankingDataArray;
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
		RankingDataArray = DataToLoad->RankingDataArray;
	}
	else if (!UGameplayStatics::DoesSaveGameExist(SaveSlotName, SaveUserIndex))
	{
		// デフォルトの保存ファイルを作成する
		CreateSaveFile();
	}
}