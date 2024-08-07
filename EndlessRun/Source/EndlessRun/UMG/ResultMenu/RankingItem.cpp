// Fill out your copyright notice in the Description page of Project Settings.


#include "EndlessRun/UMG/ResultMenu/RankingItem.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetTextLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetStringLibrary.h"

bool URankingItem::Initialize()
{
	bool Success = Super::Initialize();

	if (!Success) return false;

	TextBlock_Rank->TextDelegate.BindUFunction(this, "SetTextBlockRank");
	TextBlock_Score->TextDelegate.BindUFunction(this, "SetTextBlockScore");
	TextBlock_Time->TextDelegate.BindUFunction(this, "SetTextBlockTime");

	return true;
}

void URankingItem::SetUpRankingItem(FRankingData RankingData, int Rank)
{
	CurrentRankingData = RankingData;
	CurrentRank = Rank;

	bIsValidRank = (Rank != -1) ? true : false; // ランクが正しいか？
}

FText URankingItem::SetTextBlockRank()
{
	if (bIsValidRank == true)
	{
		return UKismetTextLibrary::Conv_IntToText(CurrentRank);
	}
	else
	{
		return FText::FromString(TEXT(""));
	}
}

FText URankingItem::SetTextBlockScore()
{
	return UKismetTextLibrary::Conv_IntToText(CurrentRankingData.Score);
}

FText URankingItem::SetTextBlockTime()
{
	// タイムを表示する
	FString Distance = UKismetStringLibrary::Conv_IntToString(UKismetMathLibrary::FTrunc(CurrentRankingData.Distance));
	FString Meter = TEXT("m");
	FString DistanceToString = UKismetStringLibrary::Concat_StrStr(Distance, Meter);
	return UKismetTextLibrary::Conv_StringToText(DistanceToString);
}
