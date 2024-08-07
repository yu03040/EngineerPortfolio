// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EndlessRun/SaveGame/RankingData.h"
#include "ResultMenu.generated.h"

class UBackgroundBlur;
class UImage;
class UScaleBox;

/**
 *
 */
UCLASS()
class ENDLESSRUN_API UResultMenu : public UUserWidget
{
	GENERATED_BODY()

	// 特殊エフェクト：ぼかし
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBackgroundBlur> BackgroundBlur;

	// 画像
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UImage> Image;

	// 
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UScaleBox> Goal_ScaleBox;

	// ランキングデータが表示される領域
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UScaleBox> Ranking_ScaleBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ranking", meta = (AllowPrivateAccess = "true"))
	FRankingData RankingData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ranking", meta = (AllowPrivateAccess = "true"))
	TArray<FRankingData> RankingDataAll;

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	FRankingData GetRankingData() { return RankingData; }

	UFUNCTION()
	void SetRankingDataAll(TArray<FRankingData> NewRankingDataAll) { RankingDataAll = NewRankingDataAll; }
};
