// Fill out your copyright notice in the Description page of Project Settings.


#include "EndlessRun/GameModes/RunGameMode.h"
#include "EndlessRun/Characters/RunCharacter.h"
#include "EndlessRun/Floors/FloorTileBase.h"
#include "EndlessRun/UMG/ResultMenu/ResultMenu.h"
#include "EndlessRun/UMG/ResultMenu/Ranking.h"
#include "EndlessRun/UMG/GamePlaying/RunScore.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Sound/SoundBase.h"

ARunGameMode::ARunGameMode()
{
	NextAttachPoint = UKismetMathLibrary::MakeTransform(
		FVector(0.0f, 0.0f, 0.0f),
		FRotator(0.0f, 0.0f, 0.0f),
		FVector(1.0f, 1.0f, 1.0f)
	);
	CurrentStraights = 0;
	MoveSeconds = 0.0f;
	SumDistance = 0.0f;
	TotalCoins = 0;
	SaveSlotName = TEXT("EndlessRun_1");
	SaveUserIndex = 0;
}

void ARunGameMode::BeginPlay()
{
	Super::BeginPlay();

	FString Path = TEXT("/Game/EndlessRun/Blueprints/UMG/WBP_RunScore_v2.WBP_RunScore_v2_C");
	TSubclassOf<UUserWidget> WidgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*Path)).LoadSynchronous();

	TObjectPtr<APlayerController> PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (WidgetClass && PlayerController)
	{
		TObjectPtr<URunScore> RunScoreWidget = Cast<URunScore>(UWidgetBlueprintLibrary::Create(this, WidgetClass, PlayerController));
		RunScoreWidget->AddToViewport();
	}

	for (int i = 0; i < 10; i++)
	{
		AddFloorTile();
	}
}

void ARunGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RunDistance(DeltaTime);
}

void ARunGameMode::AddFloorTile()
{
	if (CurrentStraights > 7)
	{
		AddFloorTileCurve();
	}
	else
	{
		int LastIndex = FloorTiles.Num() - 1;
		int Min = 0;
		int Max = LastIndex;
		int CurrentIndex = UKismetMathLibrary::RandomIntegerInRange(Min, Max);
		TObjectPtr<AActor> Floor = GetWorld()->SpawnActor<AActor>(FloorTiles[CurrentIndex], NextAttachPoint);

		TObjectPtr<AFloorTileBase> FloorBase = Cast<AFloorTileBase>(Floor);
		if (FloorBase)
		{
			NextAttachPoint = FloorBase->GetAttachPoint();
			UpdateStraightsFloor();
		}
	}
}

void ARunGameMode::AddFloorTileCurve()
{
	int LastIndex = FloorCurves.Num() - 1;
	int Min = 0;
	int Max = LastIndex;
	int CurrentIndex = UKismetMathLibrary::RandomIntegerInRange(Min, Max);
	TObjectPtr<AActor> Floor = GetWorld()->SpawnActor<AActor>(FloorCurves[CurrentIndex], NextAttachPoint);

	TObjectPtr<AFloorTileBase> FloorBase = Cast<AFloorTileBase>(Floor);
	if (FloorBase)
	{
		NextAttachPoint = FloorBase->GetAttachPoint();
		InitStraightsFloor();
	}
}

void ARunGameMode::AddCoins()
{
	TotalCoins += 1;
}

void ARunGameMode::RunDistance(float DeltaSeconds)
{
	// 1 フレーム = 1/30[s] だから 秒に変換して足す
	MoveSeconds += DeltaSeconds * 30.0f;

	auto Player = Cast<ARunCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (Player)
	{
		// xy 方向の走った距離を求める
		SumDistance = UKismetMathLibrary::VSizeXY(Player->GetActorForwardVector() * MoveSeconds);
	}
}

void ARunGameMode::UpdateStraightsFloor()
{
	CurrentStraights += 1;
}

void ARunGameMode::InitStraightsFloor()
{
	CurrentStraights = 0;
}

void ARunGameMode::ShowResult_Implementation()
{
	// マウスを表示する
	UGameplayStatics::GetPlayerController(this, 0)->bShowMouseCursor = true;

	SetActorTickEnabled(false);
	TArray<FRankingData> Load = LoadData();

	FString Path = TEXT("/Game/EndlessRun/Blueprints/UMG/WBP_Result_v2.WBP_Result_v2_C");
	TSubclassOf<UUserWidget> WidgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*Path)).LoadSynchronous();

	TObjectPtr<APlayerController> PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (WidgetClass && PlayerController)
	{
		TObjectPtr<UResultMenu> ResultMenuWidget = Cast<UResultMenu>(UWidgetBlueprintLibrary::Create(this, WidgetClass, PlayerController));
		RankingData.Score = TotalCoins;
		RankingData.Distance = SumDistance;
		ResultMenuWidget->GetRankingData().SetScore(RankingData.Score);
		ResultMenuWidget->GetRankingData().SetDistance(RankingData.Distance);
		ResultMenuWidget->SetRankingDataAll(Load);
		ResultMenuWidget->AddToViewport();
	}
}

TArray<FRankingData> ARunGameMode::LoadData()
{
	SaveGameRef = Cast<USaveGameBase>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, SaveUserIndex));
	if (SaveGameRef)
	{
		return SaveGameRef->GetRankingData();
	}
	
	return TArray<FRankingData>();
}

void ARunGameMode::SaveData(TArray<FRankingData> NewRankingData)
{
	SaveGameRef = Cast<USaveGameBase>(UGameplayStatics::CreateSaveGameObject(USaveGameBase::StaticClass()));
	if (SaveGameRef)
	{
		for (auto rank : NewRankingData)
		{
			SaveGameRef->GetRankingData().Add(rank);
		}

		UGameplayStatics::SaveGameToSlot(SaveGameRef, SaveSlotName, SaveUserIndex);
	}
}