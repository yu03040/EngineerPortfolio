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
#include "Kismet/KismetSystemLibrary.h"

void URanking::NativeConstruct()
{
	Super::NativeConstruct();

	// BackToTitle_Button の OnClicked に「OnClickedBackToTitle_Button」を関連づける
	BackToTitle_Button->OnClicked.AddUniqueDynamic(this, &URanking::OnClickedBackToTitle_Button);
}

void URanking::OnClickedBackToTitle_Button()
{
	UGameplayStatics::PlaySound2D(GetWorld(), ClickSound);

	// ポーズ中のため Delay を使用
	FLatentActionInfo LatentInfo;
	LatentInfo.Linkage = 0;
	LatentInfo.UUID = FMath::Rand();
	LatentInfo.ExecutionFunction = FName("BackTitle");
	LatentInfo.CallbackTarget = this;

	UKismetSystemLibrary::Delay(GetWorld(), 0.5f, LatentInfo);
}

void URanking::BackTitle()
{
	FName LevelName = TEXT("TitleMap");
	UGameplayStatics::OpenLevel(this, LevelName);
}

void URanking::SetUpRankingData(FRankingData& CurrentRankingData, TArray<FRankingData>& RankingDataAll)
{
	// WidgetBlueprint の Class を取得する
	FString Path = TEXT("/Game/EndlessRun/Blueprints/UMG/WBP_RankingItem_v2.WBP_RankingItem_v2_C");
	TSubclassOf<UUserWidget> RankingItemWidgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*Path)).LoadSynchronous();

	// PlayerController を取得する
	TObjectPtr<APlayerController> PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	// 自分データを YourScore_ScaleBox に反映する
	DisplayRankingItemInScaleBox(RankingItemWidgetClass, PlayerController, CurrentRankingData);

	// 自分データをランキングに追加
	RankingDataAll.Add(CurrentRankingData);

	// ランキングをソートする
	RankingDataAll.StableSort([](const FRankingData& A, const FRankingData& B){
		// コインが同じだったら距離が大きい方で比較
		if (A.Score == B.Score)
		{
			return A.Distance > B.Distance;
		}
		return A.Score > B.Score;
	});

	// ベスト 5 まで表示
	if (RankingDataAll.Num() > 5)
	{
		RankingDataAll.RemoveAt(5);
	}

	// 他の人のデータを RankingList_VerticalBox に反映
	DisplayRankingItemInVerticalBox(RankingDataAll, RankingItemWidgetClass, PlayerController);

	TObjectPtr<ARunGameMode> GameMode = Cast<ARunGameMode>(UGameplayStatics::GetGameMode(this));
	if (GameMode)
	{
		// ランキングデータを更新して、セーブする
		GameMode->RankingDataArray = RankingDataAll;
		GameMode->SaveGame();
	}
}

void URanking::DisplayRankingItemInVerticalBox(TArray<FRankingData>& RankingDataAll, TSubclassOf<UUserWidget>& RankingItemWidgetClass, TObjectPtr<APlayerController>& PlayerController)
{
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
}

void URanking::DisplayRankingItemInScaleBox(TSubclassOf<UUserWidget>& RankingItemWidgetClass, TObjectPtr<APlayerController>& PlayerController, FRankingData& CurrentRankingData)
{
	// WidgetClassとPlayerControllerが取得できたか判定する
	if (RankingItemWidgetClass && PlayerController)
	{
		// RankingItem 用のウィジェットを作成する
		TObjectPtr<URankingItem> RankingItemWidget = CreateWidget<URankingItem>(PlayerController, RankingItemWidgetClass);
		if (RankingItemWidget)
		{
			RankingItemWidget->SetUpRankingItem(CurrentRankingData, -1);

			// RankingItem を Ranking の子として表示する
			YourScore_ScaleBox->AddChild(RankingItemWidget);
		}

	}
}