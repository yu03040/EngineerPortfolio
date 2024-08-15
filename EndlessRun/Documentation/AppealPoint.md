# 制作目的
以前、ブループリントで制作したゲームをC++で再構築し、Unreal Engineでのシューティングゲーム開発経験を活かして、さらに深い理解を得ることを目的としていました。このプロジェクトでは、「EndlessRun」というランゲームを制作し、ゲームプレイとランキング機能を中心に、コードベースでのゲーム開発に挑戦しました。

# アピールポイント
ランキング機能の実装に特に力を入れました。プレイヤーが集めたコインと走った距離を基に、順位を決定し、ランキング画面に反映させる仕組みを作り上げました。各ゲームセッションの結果を保存し、それをランキングに反映する機能も実装しました。また、UIとの連携を考慮し、結果画面やランキングアイテムを動的に生成・表示することにも注力しました。

# 苦労した点
ランキングデータの保存とロードに苦戦しました。  
- 一つ目の原因は、セーブする対象のプロパティに`UPROPERTY`を付け忘れていたことです。これにより、データが正しく保存されず、予期せぬ動作を引き起こしていました。

- ```cpp
	// SaveGame.h
	// Fill out your copyright notice in the Description page of Project Settings.

	#pragma once

	#include "CoreMinimal.h"
	#include "GameFramework/SaveGame.h"
	#include "RankingData.h"
	#include "SaveGameBase.generated.h"

	/**
	 *
	 */
	UCLASS()
	class ENDLESSRUN_API USaveGameBase : public USaveGame
	{
		GENERATED_BODY()

	public:
		// RankingData の配列
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveGame")
		TArray<FRankingData> RankingDataArray;
	};
  ```


 
- 二つ目の原因は、`Ranking`クラスで更新したランキングデータを、`GameMode`クラスの`LoadRankingData`に渡していなかったことです。これにより、ランキングが正しく更新されず、ゲームプレイに一貫性が欠ける問題が発生しました。

- ```cpp
	// Ranking.cpp
	// Fill out your copyright notice in the Description page of Project Settings.

	// ... 略

	void URanking::SetUpRankingData(FRankingData& CurrentRankingData, TArray<FRankingData>& RankingDataAll)
	{
		// ... 略

		TObjectPtr<ARunGameMode> GameMode = Cast<ARunGameMode>(UGameplayStatics::GetGameMode(this));
		if (GameMode)
		{
			// ランキングデータを更新して、セーブする
			GameMode->LoadRankingData = RankingDataAll;
			GameMode->SaveGame();
		}
	}

	// ... 略
  ```



この経験を通じて、データ管理とC++におけるUnreal Engineのセーブシステムの重要性を深く理解することができました。