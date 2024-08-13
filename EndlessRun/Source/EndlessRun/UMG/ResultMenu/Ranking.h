// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EndlessRun/SaveGame/RankingData.h"
#include "Ranking.generated.h"

class UVerticalBox;
class USizeBox;
class UTextBlock;
class UHorizontalBox;
class USpacer;
class UScaleBox;
class UButton;

/**
 * 
 */
UCLASS()
class ENDLESSRUN_API URanking : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UVerticalBox> VerticalBox_0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USizeBox> SizeBox_0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UVerticalBox> VerticalBox_1;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> TextBlock_Ranking;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UHorizontalBox> HorizontalBox_0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpacer> Spacer_0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USizeBox> SizeBox_1;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> TextBlock_Rank;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpacer> Spacer_1;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USizeBox> SizeBox_2;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> TextBlock_Score;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpacer> Spacer_2;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USizeBox> SizeBox_3;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> TextBlock_Mileage;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpacer> Spacer_3;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UScaleBox> ScaleBox_0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UVerticalBox> RankingList_VerticalBox;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USizeBox> SizeBox_4;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UVerticalBox> VerticalBox_2;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> TextBlock_YourScore;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UScaleBox> YourScore_ScaleBox;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UButton> BackToTitle_Button;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> TextBlock_BackToTitle;

protected:
	// クリック音
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	TObjectPtr<USoundBase> ClickSound;

protected:
	virtual void NativeConstruct() override;

public:
	// BackToTitle_Button の OnClicked イベントに関連づける関数
	UFUNCTION()
	void OnClickedBackToTitle_Button();
 
	UFUNCTION()
	void BackTitle();

	/**
	* ランキングを更新する関数
	* @param CurrentRankingData 現在の RankingData の参照
	* @param RankingDataAll 過去の RankingData の配列の参照
	*/
	void SetUpRankingData(FRankingData& CurrentRankingData, TArray<FRankingData>& RankingDataAll);

	/**
	* ランキングを表示する関数
	* @param RankingDataAll 過去の RankingData の配列の参照
	* @param RankingItemWidgetClass RankingItem クラスの参照
	* @param PlayerController プレイヤーコントローラーの参照
	*/
	void DisplayRankingItemInVerticalBox(TArray<FRankingData>& RankingDataAll, TSubclassOf<UUserWidget>& RankingItemWidgetClass, TObjectPtr<APlayerController>& PlayerController);

	/**
	* 自分のスコアを表示する関数
	* @param RankingItemWidgetClass RankingItem クラスの参照
	* @param PlayerController プレイヤーコントローラーの参照
	* @param CurrentRankingData 現在の RankingData の参照
	*/
	void DisplayRankingItemInScaleBox(TSubclassOf<UUserWidget>& RankingItemWidgetClass, TObjectPtr<APlayerController>& PlayerController, FRankingData& CurrentRankingData);
};
