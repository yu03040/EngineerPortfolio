// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "UICharacter.generated.h"

/**
 *
 */
UCLASS()
class GUNMAN_API UUICharacter : public UUserWidget
{
	GENERATED_BODY()

public:


protected:

	virtual void NativeConstruct() override;

	bool Initialize() override;

	/** 体力バー */
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* Health_ProgressBar;

	/** Killのテキスト */
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Kill_TextBlock;

	/** 倒した数を表示するテキスト */
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* KillCount_TextBlock;

	/** 倒した数のアニメーション */
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* MoveKillCount;

private:
	// 倒した数を表示する関数
	UFUNCTION()
	FText SetKillCountText();

	// 体力を表示する関数
	UFUNCTION()
	float SetHealthProgressBar();
};
