// Fill out your copyright notice in the Description page of Project Settings.


#include "EndlessRun/UMG/ResultMenu/Ranking.h"
#include "EndlessRun/GameModes/RunGameMode.h"
#include "RankingItem.h"
#include "Components/Button.h"
#include "Components/ScaleBox.h"
#include "Components/VerticalBox.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

void URanking::NativeConstruct()
{
	Super::NativeConstruct();

	// BackToTitle ボタンが押されるときのイベント
	BackToTitle_Button->OnClicked.AddUniqueDynamic(this, &URanking::OnClickedBackToTitle_Button);
}

void URanking::OnClickedBackToTitle_Button()
{
	// クリック音を鳴らす
	StartSound();

	//1秒後にタイトルに戻る
	float DelayTime = 1.0f;
	FLatentActionInfo LatentActionInfo;
	LatentActionInfo.CallbackTarget = this;
	LatentActionInfo.ExecutionFunction = FName(TEXT(""));
	LatentActionInfo.Linkage = 0;
	UKismetSystemLibrary::Delay(this, DelayTime, LatentActionInfo);

	FName LevelName = TEXT("/Game/EndlessRun/Maps/TitleMap.TitleMap");
	UGameplayStatics::OpenLevel(this, LevelName);
}

void URanking::SetUpRankingData(FRankingData CurrentRankingData, TArray<FRankingData>& RankingDataAll)
{
	// WidgetBlueprintのClassを取得する
	FString Path = TEXT("/Game/EndlessRun/Blueprints/UMG/WBP_RankingItem_v2.WBP_RankingItem_v2_C");
	TSubclassOf<UUserWidget> RankingItemWidgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*Path)).LoadSynchronous();

	// PlayerControllerを取得する
	TObjectPtr<APlayerController> PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	// WidgetClassとPlayerControllerが取得できたか判定する
	if (RankingItemWidgetClass && PlayerController)
	{
		// RankingItem を子として追加表示する
		TObjectPtr<URankingItem> RankingItemWidget = CreateWidget<URankingItem>(PlayerController, RankingItemWidgetClass);
		if (RankingItemWidget)
		{
			RankingItemWidget->SetUpRankingItem(CurrentRankingData, -1);
			YourScore_ScaleBox->AddChild(RankingItemWidget);
		}

	}

	for (FRankingData RankingData : RankingDataAll)
	{
		int index = 0;

		// 現在のランクと過去のランクを比較
		if (CurrentRankingData.Score >= RankingData.Score)
		{
			RankingDataAll.Insert(CurrentRankingData, index);
			break; // 挿入した時点で終了
		}
		index++;
	}

	// ランキングに同じ値がなければ、配列の最下位に追加
	RankingDataAll.AddUnique(CurrentRankingData);

	// 他の 5 人分のデータを反映
	for (int i = 0; i < RankingDataAll.Num(); i++)
	{
		if (RankingItemWidgetClass && PlayerController)
		{
			// RankingItem を子として追加表示する
			TObjectPtr<URankingItem> RankingItemWidget = CreateWidget<URankingItem>(PlayerController, RankingItemWidgetClass);
			if (RankingItemWidget)
			{
				RankingList_VerticalBox->AddChild(RankingItemWidget);
				RankingItemWidget->SetUpRankingItem(RankingDataAll[i], i + 1);
			}
		}
	}

	TObjectPtr<ARunGameMode> GameMode = Cast<ARunGameMode>(UGameplayStatics::GetGameMode(this));
	if (GameMode)
	{
		GameMode->SaveData(RankingDataAll);
	}
}

void URanking::StartSound_Implementation()
{

}
