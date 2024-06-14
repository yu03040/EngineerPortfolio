// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIGameOver.generated.h"

/**
 *
 */
UCLASS()
class GUNMAN_API UUIGameOver : public UUserWidget
{
	GENERATED_BODY()

protected:
	void NativeConstruct() override;

	/** コンティニューボタン */
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> Continue_Button;

	/** コンティニューボタンのテキスト */
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UTextBlock> Continue_TextBlock;

	/** ゲーム終了ボタン */
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> GameEnd_Button;

	/** ゲーム終了ボタンのテキスト */
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UTextBlock> GameEnd_TextBlock;

public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	/** ゲームを終了する関数 */
	UFUNCTION()
	void OnClickedContinue_Button();

	/** ゲームを終了する関数 */
	UFUNCTION()
	void OnClickedGameEnd_Button();

	UFUNCTION(BlueprintPure, Category = "Button")
	UButton* GetContinue_Button() const { return Continue_Button; }

	UFUNCTION(BlueprintPure, Category = "Button")
	UButton* GetGameEnd_Button() const { return GameEnd_Button; }
};
