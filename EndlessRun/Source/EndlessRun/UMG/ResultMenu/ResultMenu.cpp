// Fill out your copyright notice in the Description page of Project Settings.


#include "EndlessRun/UMG/ResultMenu/ResultMenu.h"
#include "Ranking.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/ScaleBox.h"

void UResultMenu::NativeConstruct()
{
	Super::NativeConstruct();

	// マウスカーソルをオンにする
	TObjectPtr<APlayerController> Controller = UGameplayStatics::GetPlayerController(this, 0);
	Controller->bShowMouseCursor = true;

	// ゲームを一時停止する
	UGameplayStatics::SetGamePaused(this, true);

	// ゲームの入力を止める
	Controller->DisableInput(Controller);

	// UI の操作を可能にする
	UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(Controller, NULL, EMouseLockMode::DoNotLock, true, false);

	// ランキングを表示する
	DisplayRanking();
}

void UResultMenu::DisplayRanking()
{
	// WidgetBlueprint の Class を取得する
	FString Path = TEXT("/Game/EndlessRun/Blueprints/UMG/WBP_Ranking_v2.WBP_Ranking_v2_C");
	TSubclassOf<UUserWidget> RankingWidgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*Path)).LoadSynchronous();

	// PlayerController を取得する
	TObjectPtr<APlayerController> PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	// WidgetClassとPlayerControllerが取得できたか判定する
	if (RankingWidgetClass && PlayerController)
	{
		// ランキング用のウィジェットを作成する
		TObjectPtr<URanking> RankingWidget = Cast<URanking>(UWidgetBlueprintLibrary::Create(this, RankingWidgetClass, PlayerController));
		if (RankingWidget)
		{
			// Ranking を ResultMenu の子として表示する
			Ranking_ScaleBox->AddChild(RankingWidget);

			// ランキングを更新する
			RankingWidget->SetUpRankingData(RankingData, RankingDataAll);
		}
	}
}
