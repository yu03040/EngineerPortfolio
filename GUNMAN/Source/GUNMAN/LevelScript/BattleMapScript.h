// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "BaseMapScript.h"
#include "BattleMapScript.generated.h"

/**
 *
 */
UCLASS()
class GUNMAN_API ABattleMapScript : public ABaseMapScript
{
	GENERATED_BODY()

public:
	ABattleMapScript();

	/* ボタンの選択状態から非選択状態に遷移するときのボタンの色を変更する関数 */
	virtual void ChangeButtonColor() override;

	/* 選択されたボタンによって出力を変える関数 */
	virtual void UpdateOutputButton() override;

	/** ボタンの位置を初期化する関数 */
	void InitializeButtonPosition();

protected:
	virtual void BeginPlay() override;

	/* ブループリントのプレイ中のウィジェット */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> HUD_WidgetClass;

	/* UI_PaseMenu の Ref */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	class UUI_PaseMenu* UI_PaseMenu;

	/* UICharacter の Ref */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	class UUICharacter* UI_Character;

	/* WBP_UICharacter のパス */ 
	FString PlayUIpath = "/Game/UMG/WBP_UICharacter.WBP_UICharacter_C";
};
