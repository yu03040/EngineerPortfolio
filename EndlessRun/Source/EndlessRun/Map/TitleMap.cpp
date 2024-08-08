// Fill out your copyright notice in the Description page of Project Settings.


#include "EndlessRun/Map/TitleMap.h"
#include "EndlessRun/UMG/TitleMenu/TitleMenu.h"
#include "Kismet/GameplayStatics.h"

ATitleMap::ATitleMap()
{
}

void ATitleMap::BeginPlay()
{
	Super::BeginPlay();

	FString Path = TEXT("/Game/EndlessRun/Blueprints/UMG/WBP_Title_v2.WBP_Title_v2_C");
	TSubclassOf<UUserWidget> WidgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*Path)).LoadSynchronous();

	// PlayerControllerを取得する
	TObjectPtr<APlayerController> PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	// WidgetClassとPlayerControllerが取得できたか判定する
	if (WidgetClass && PlayerController)
	{
		// RankingItem を子として追加表示する
		TitleWidget = CreateWidget<UTitleMenu>(PlayerController, WidgetClass);
		if (TitleWidget)
		{
			TitleWidget->AddToViewport();
			TitleWidget->OnGameStarted.AddDynamic(this, &ATitleMap::GameStarted);
		}

	}
}

void ATitleMap::GameStarted()
{
	if (TitleWidget)
	{
		TitleWidget->RemoveFromParent();
		// ゲームを開始する
		FName LevelName = TEXT("RunGameMap");
		UGameplayStatics::OpenLevel(this, LevelName);
	}
}
