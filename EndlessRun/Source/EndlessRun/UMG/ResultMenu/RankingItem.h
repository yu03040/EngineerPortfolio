// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EndlessRun/SaveGame/RankingData.h"
#include "RankingItem.generated.h"

class UOverlay;
class UHorizontalBox;
class USpacer;
class USizeBox;
class UTextBlock;

/**
 * 
 */
UCLASS()
class ENDLESSRUN_API URankingItem : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UOverlay> Overlay;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UHorizontalBox> HorizontalBox;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpacer> Spacer_0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USizeBox> SizeBox_0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> TextBlock_Rank;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpacer> Spacer_1;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USizeBox> SizeBox_1;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> TextBlock_Score;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpacer> Spacer_2;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USizeBox> SizeBox_2;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> TextBlock_Distance;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpacer> Spacer_3;

	// ランクが正しい場合は true, そうでない場合は flase
	bool bIsValidRank;

public:
	/**
	* RankingItem を更新する関数
	* @param RankingData RankingData の参照
	* @param Rank 順位（Rank が -1 になるときは「YourScore」のRankingItem を更新する）
	*/
	void SetUpRankingItem(FRankingData& RankingData, int Rank);

	/**
	* 条件に応じて異なる Text を選択して出力する関数
	* @param bCondition 条件
	* @param Option_True true の場合、この Text を選択する
	* @param Option_False flase の場合、この Text を選択する
	*/
	FText RankSelect(bool bCondition, FText Option_True, FText Option_False);
};
