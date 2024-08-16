// Fill out your copyright notice in the Description page of Project Settings.


#include "GUNMAN/GameModes/GameOverMode.h"
#include "Kismet/GameplayStatics.h"
#include "GUNMAN/UMG/UIGameOver.h"

void AGameOverMode::BeginPlay()
{
	Super::BeginPlay();

	DisplayGameOver();
}

void AGameOverMode::DisplayGameOver()
{
	// WidgetBlueprint の Class を取得する
	FString Path = TEXT("/Game/GUNMAN/Blueprint/UMG/WBP_GameOver.WBP_GameOver_C");
	TSubclassOf<UUserWidget> WidgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*Path)).LoadSynchronous();

	// PlayerController を取得する
	TObjectPtr<APlayerController> PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	// WidgetClass と PlayerController が取得できたか判定する
	if (WidgetClass && PlayerController)
	{
		// ゲームオーバー用のウィジェットを作成する
		TObjectPtr<UUIGameOver> RunScoreWidget = CreateWidget<UUIGameOver>(PlayerController, WidgetClass);
		if (RunScoreWidget)
		{
			// ゲームオーバーを画面に表示する
			RunScoreWidget->AddToViewport();
		}
	}
}
