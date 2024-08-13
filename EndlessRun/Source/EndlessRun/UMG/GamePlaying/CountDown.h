// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TimerManager.h"
#include "CountDown.generated.h"

class UVerticalBox;
class UHorizontalBox;
class UTextBlock;
class USpacer;

/**
 * 
 */
UCLASS()
class ENDLESSRUN_API UCountDown : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UVerticalBox> VerticalBox;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UHorizontalBox> HorizontalBox_0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> TextBlock_3;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpacer> Spacer_0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> TextBlock_2;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpacer> Spacer_1;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> TextBlock_1;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UHorizontalBox> HorizontalBox_1;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpacer> Spacer_2;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> TextBlock_GO;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpacer> Spacer_3;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UTextBlock>> CountTexts;

	// タイマーを一意に識別するためのハンドル
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	FTimerHandle TimerHandle;

	int CountDownTime = 3.0;

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void SetTextBlockOpacity(UTextBlock* TextBlock, float Opacity);

	// カウントダウンを更新する関数
	UFUNCTION()
	void UpdateCountdown();
};
