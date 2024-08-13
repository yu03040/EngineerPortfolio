// Fill out your copyright notice in the Description page of Project Settings.


#include "EndlessRun/UMG/ResultMenu/RankingItem.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetTextLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetStringLibrary.h"

void URankingItem::SetUpRankingItem(FRankingData& RankingData, int Rank)
{
	bIsValidRank = (Rank != -1) ? true : false; // ランクが正しいか？

	// ランクを更新
	TextBlock_Rank->SetText(RankSelect(bIsValidRank, FText::AsNumber(Rank), FText()));

	// スコアを更新
	TextBlock_Score->SetText(FText::AsNumber(RankingData.Score));

	// 距離を更新
	FText Distance = FText::Format(FText::FromString(TEXT("{0} m")), FText::AsNumber(FMath::FloorToInt(RankingData.Distance)));
	TextBlock_Distance->SetText(Distance);
}

FText URankingItem::RankSelect(bool bCondition, FText Option_True, FText Option_False)
{
	return bCondition ? Option_True : Option_False;
}