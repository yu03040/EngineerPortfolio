// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EndlessRun/SaveGame/SaveGameBase.h"
#include "EndlessRun/SaveGame/RankingData.h"
#include "RunGameMode.generated.h"

/**
 *
 */
UCLASS()
class ENDLESSRUN_API ARunGameMode : public AGameModeBase
{
	GENERATED_BODY()

private:
	// 次の床が発生する場所
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FloorTile", meta = (AllowPrivateAccess = "true"))
	FTransform NextAttachPoint;

	// まっすぐな床の配列
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FloorTile", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<AActor>> FloorTiles;

	// 曲がった床の配列
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FloorTile", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<AActor>> FloorCurves;

	// まっすぐな床のカウンター
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FloorTile", meta = (AllowPrivateAccess = "true"))
	int CurrentStraights;

	// プレイヤーが走った秒数
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FloorTile", meta = (AllowPrivateAccess = "true"))
	float MoveSeconds;

	// プレイヤーが走った距離
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FloorTile", meta = (AllowPrivateAccess = "true"))
	float SumDistance;

	// プレイヤーが取ったコインの数
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FloorTile", meta = (AllowPrivateAccess = "true"))
	int TotalCoins;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveGame", meta = (AllowPrivateAccess = "true"))
	FString SaveSlotName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveGame", meta = (AllowPrivateAccess = "true"))
	int SaveUserIndex;

	// ランキング
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ranking", meta = (AllowPrivateAccess = "true"))
	FRankingData RankingData;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ranking", meta = (AllowPrivateAccess = "true"))
	TArray<FRankingData> RankingDataArray;

protected:
	// ゲーム開始時またはスポーン時にコールされる
	virtual void BeginPlay() override;

public:
	// このゲームモードのプロパティのデフォルト値を設定する（コンストラクター）
	ARunGameMode();

	// フレームごとに呼び出される
	virtual void Tick(float DeltaTime) override;

	// 取ったコインと走った距離を表示する関数
	void DisplayRunScore();

	// まっすぐな床を生成する関数
	void AddFloorTile();

	// 何もスポーンしないまっすぐな床を生成する関数
	void AddNothingSpawnFloorTile();

	// 曲がった床を生成する関数
	void AddFloorTileCurve();

	// 取ったコインをカウントする関数
	void AddCoins();

	// 結果を表示する関数
	void ShowResult();

	/**
	*  走った距離を計算する関数
	*  @param DeltaSeconds フレーム間の合計時間
	*  @param Player プレイヤーの参照
	*/
	void RunDistance(float DeltaSeconds, TObjectPtr<class ARunCharacter>& Player);

	// まっすぐな床の枚数を更新する関数
	void UpdateStraightsFloor();

	// まっすぐな床の枚数を初期化する関数
	void InitStraightsFloor();

	int GetTotalCoins() const { return TotalCoins; }

	float GetSumDistance() const { return SumDistance; }

	void CreateSaveFile();

	void SaveGame();

	void LoadGame();
};
