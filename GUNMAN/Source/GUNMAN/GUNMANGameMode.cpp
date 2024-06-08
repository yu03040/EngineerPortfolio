// Copyright Epic Games, Inc. All Rights Reserved.

#include "GUNMANGameMode.h"
#include "GUNMANCharacter.h"
#include "GUNMANController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "UMG/UITimeLimitWidget.h"
#include "UObject/ConstructorHelpers.h"

AGUNMANGameMode::AGUNMANGameMode() : Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprint/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	PlayerControllerClass = AGUNMANController::StaticClass();

	GameClearKillCount = 10;
}

void AGUNMANGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 予め決めた制限時間から経過時間を引く
	UITimeLimitRef->Time -= DeltaTime;
	// 0 秒以下ならゲーム終了（ゲームオーバー）
	if (UITimeLimitRef->Time <= 0.0f)
	{
		UGameplayStatics::OpenLevel(this, "GameOverMap");
	}

	// 1敵を10体以上倒したらゲームクリア
	if (PlayerRef->GetKillCount() >= GameClearKillCount)
	{
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AGUNMANGameMode::OpenGameClearMap, 2.0f);
	}
}

void AGUNMANGameMode::BeginPlay()
{
	Super::BeginPlay();

	// プレイヤ―の情報をもらう
	auto IsPlayer = IsValid(UGameplayStatics::GetPlayerCharacter(this->GetWorld(), 0));
	if (IsPlayer == true)
	{
		PlayerRef = Cast<AGUNMANCharacter>(UGameplayStatics::GetPlayerCharacter(this->GetWorld(), 0));
	}

	// プレイヤーコントローラーを取得
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	// ウィジェットブループリントのパスをセット
	FString path = "/Game/UMG/WBP_TimeLimit.WBP_TimeLimit_C";
	// アセットパスから UserWidgetClass を生成する
	WidgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*path)).LoadSynchronous();
	// タイトルのウィジェットを作成
	if (IsValid(WidgetClass))
	{

		UITimeLimitRef = Cast<UUITimeLimitWidget>(CreateWidget(PlayerController, WidgetClass));

		if (UITimeLimitRef)
		{
			UITimeLimitRef->AddToViewport();
		}
	}
}

void AGUNMANGameMode::OpenGameClearMap()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("GameClearMap")));
}
