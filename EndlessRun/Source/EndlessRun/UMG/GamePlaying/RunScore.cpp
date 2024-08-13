// Fill out your copyright notice in the Description page of Project Settings.


#include "EndlessRun/UMG/GamePlaying/RunScore.h"
#include "EndlessRun/GameModes/RunGameMode.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetTextLibrary.h"
#include "Kismet/KismetMathLibrary.h"

void URunScore::NativeConstruct()
{
	Super::NativeConstruct();
}

bool URunScore::Initialize()
{
	bool bSuccess = Super::Initialize();

	if (!bSuccess) return false;

	// TextBlock の値に関数を Bind する
	TextBlock_CoinsCount->TextDelegate.BindUFunction(this, "SetTextBlockCoinsCount");
	TextBlock_MileageMeter->TextDelegate.BindUFunction(this, "SetTextBlockMileageMeter");

	return true;
}

FText URunScore::SetTextBlockCoinsCount()
{
	TObjectPtr<ARunGameMode> GameMode = Cast<ARunGameMode>(UGameplayStatics::GetGameMode(this));
	if (GameMode)
	{
		return UKismetTextLibrary::Conv_IntToText(GameMode->GetTotalCoins());
	}
	else
	{
		return FText();
	}
}

FText URunScore::SetTextBlockMileageMeter()
{
	TObjectPtr<ARunGameMode> GameMode = Cast<ARunGameMode>(UGameplayStatics::GetGameMode(this));
	if (GameMode)
	{
		return UKismetTextLibrary::Conv_IntToText(UKismetMathLibrary::FTrunc(GameMode->GetSumDistance()));
	}
	else
	{
		return FText();
	}
}
