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
	bool bPaused = true;
	UGameplayStatics::SetGamePaused(this, bPaused);

	// ゲームの入力を止める
	Controller->DisableInput(Controller);

	// UI の操作を可能にする
	UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(Controller, NULL, EMouseLockMode::DoNotLock, true, false);

	FString Path = TEXT("/Game/EndlessRun/Blueprints/UMG/WBP_Ranking_v2.WBP_Ranking_v2_C");
	TSubclassOf<UUserWidget> RankingWidgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*Path)).LoadSynchronous();

	TObjectPtr<APlayerController> PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (RankingWidgetClass && PlayerController)
	{
		// RankingItem を子として追加表示する
		TObjectPtr<URanking> RankingWidget = Cast<URanking>(UWidgetBlueprintLibrary::Create(this, RankingWidgetClass, PlayerController));
		Ranking_ScaleBox->AddChild(RankingWidget);
		RankingWidget->SetUpRankingData(RankingData, RankingDataAll);
	}
}
