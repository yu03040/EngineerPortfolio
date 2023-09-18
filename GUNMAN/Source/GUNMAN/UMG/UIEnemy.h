// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIEnemy.generated.h"

/**
 * 
 */
UCLASS()
class GUNMAN_API UUIEnemy : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// コンストラクタ
	virtual void NativeConstruct() override;

	// Constructメソッド
	virtual void Construct();

	/** 敵の体力の割合 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI)
		float HealthRate = 1.0f;

protected:
	/** プレイヤーかどうか判断する変数 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI)
		ACharacter* Player;

private:
};
