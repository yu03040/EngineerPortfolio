// Fill out your copyright notice in the Description page of Project Settings.


#include "GUNMAN/GameModes/GameClearMode.h"
#include "Kismet/GameplayStatics.h"
#include "GUNMAN/UMG/UIGameClear.h"

void AGameClearMode::BeginPlay()
{
	Super::BeginPlay();

	DisplayGameClear();
}

void AGameClearMode::DisplayGameClear()
{
	// WidgetBlueprint の Class を取得する
	FString Path = TEXT("/Game/GUNMAN/Blueprint/UMG/WBP_GameClear.WBP_GameClear_C");
	TSubclassOf<UUserWidget> WidgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*Path)).LoadSynchronous();

	// PlayerController を取得する
	TObjectPtr<APlayerController> PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	// WidgetClass と PlayerController が取得できたか判定する
	if (WidgetClass && PlayerController)
	{
		// クリア用のウィジェットを作成する
		TObjectPtr<UUIGameClear> RunScoreWidget = CreateWidget<UUIGameClear>(PlayerController, WidgetClass);
		if (RunScoreWidget)
		{
			// クリアを画面に表示する
			RunScoreWidget->AddToViewport();
		}
	}
}