// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "EndlessRun/UMG/TitleMenu/TitleMenu.h"
#include "TitleMap.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSRUN_API ATitleMap : public ALevelScriptActor
{
	GENERATED_BODY()

private:
	// タイトルメニュー の参照
	TObjectPtr<UTitleMenu> TitleWidget;

protected:
	// ゲーム開始時またはスポーン時にコールされる
	virtual void BeginPlay() override;

public:
	// このレベルのプロパティのデフォルト値を設定する（コンストラクター）
	ATitleMap();

public:
	// 呼び出されるメソッド
	UFUNCTION()
	void GameStarted();
};
