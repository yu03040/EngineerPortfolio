// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UICharacter.generated.h"

/**
 *
 */
UCLASS()
class GUNMAN_API UUICharacter : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// コンストラクタ
	virtual void NativeConstruct() override;

	// Constructメソッド
	virtual void Construct();

	/** Tick の DeltaTime を格納する変数 */
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float DeltaTimes;

	/** 制限時間の秒数 */
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float TimeLimit;

protected:
	/** プレイヤーかどうか判断する変数 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay)
	ACharacter* Player;

	/** 制限時間のテキスト */
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TimeLimit_TextBlock;

	/** 制限時間の秒数を表示するテキスト */
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TimeRemoving_TextBlock;

private:

};
