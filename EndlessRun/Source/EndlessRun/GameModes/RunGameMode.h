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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FloorTile", meta = (AllowPrivateAccess = "true"))
	FTransform NextAttachPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FloorTile", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<AActor>> FloorTiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FloorTile", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<AActor>> FloorCurves;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FloorTile", meta = (AllowPrivateAccess = "true"))
	int CurrentStraights;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FloorTile", meta = (AllowPrivateAccess = "true"))
	float MoveSeconds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FloorTile", meta = (AllowPrivateAccess = "true"))
	float SumDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FloorTile", meta = (AllowPrivateAccess = "true"))
	int TotalCoins;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveGame", meta = (AllowPrivateAccess = "true"))
	FString SaveSlotName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveGame", meta = (AllowPrivateAccess = "true"))
	int SaveUserIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveGame", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USaveGameBase> SaveGameRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ranking", meta = (AllowPrivateAccess = "true"))
	FRankingData RankingData;

private:

	// 曲がった床を生成する関数
	UFUNCTION(BlueprintCallable, Category = "FloorTile")
	void AddFloorTileCurve();

	/**
	*  走った距離を計算する関数
	*  @param DeltaSeconds フレーム間の合計時間
	*/
	UFUNCTION(BlueprintCallable, Category = "FloorTile")
	void RunDistance(float DeltaSeconds);

	// まっすぐな床の枚数を更新する関数
	void UpdateStraightsFloor();

	// まっすぐな床の枚数を初期化する関数
	void InitStraightsFloor();

protected:
	virtual void BeginPlay() override;

public:
	ARunGameMode();

	virtual void Tick(float DeltaTime) override;

public:
	// 直線の床を生成する関数
	UFUNCTION(BlueprintCallable, Category = "FloorTile")
	void AddFloorTile();

	// 直線の床を生成する関数
	UFUNCTION(BlueprintCallable, Category = "FloorTile")
	void AddCoins();

	// 結果を表示する関数
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Floor")
	void ShowResult();

	// ランキングデータを読み込む関数
	UFUNCTION(BlueprintCallable, Category = "Floor")
	TArray<FRankingData> LoadData();

	// ランキングデータを保存する関数
	UFUNCTION(BlueprintCallable, Category = "Floor")
	void SaveData(TArray<FRankingData> NewRankingData);

	UFUNCTION()
	int GetTotalCoins() { return TotalCoins; }

	UFUNCTION()
	float GetSumDistance() { return SumDistance; }
};
