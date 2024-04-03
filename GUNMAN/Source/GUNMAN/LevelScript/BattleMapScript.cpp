// Fill out your copyright notice in the Description page of Project Settings.


#include "GUNMAN/LevelScript/BattleMapScript.h"
#include "GUNMAN/UMG/UI_PaseMenu.h"
#include "GUNMAN/UMG/UICharacter.h"
#include "GUNMAN/GUNMANCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

ABattleMapScript::ABattleMapScript()
{
	UI_PaseMenu = NULL;
	UI_Character = NULL;
	MaxButtonCounter = 3;
	InvalidButtonIndex = 4;
}

void ABattleMapScript::BeginPlay()
{
	Super::BeginPlay();
}

void ABattleMapScript::ChangeButtonColor()
{
	// 非選択状態の色をそれぞれ白色に
	UButton* BackToTitleButton = UI_PaseMenu->GetBackToTitle_Button();
	if (BackToTitleButton)
	{
		BackToTitleButton->SetBackgroundColor(FLinearColor::White);
	}

	UButton* CancelButton = UI_PaseMenu->GetCancel_Button();
	if (CancelButton)
	{
		CancelButton->SetBackgroundColor(FLinearColor::White);
	}

	UButton* EndButton = UI_PaseMenu->GetGameEnd_Button();
	if (EndButton)
	{
		EndButton->SetBackgroundColor(FLinearColor::White);
	}

	// 選択状態のボタンの色をそれぞれ変える
	switch (ButtonCounter)
	{
		case 1:
			BackToTitleButton->SetBackgroundColor(SelectedColor);
			break;
		case 2:
			CancelButton->SetBackgroundColor(SelectedColor);
			break;
		case 3:
			EndButton->SetBackgroundColor(SelectedColor);
			break;
	}
}

void ABattleMapScript::UpdateOutputButton()
{
	switch (ButtonCounter)
	{
		case 1:
			UI_PaseMenu->OnClickedBackToTitle_Button();
			break;
		case 2:
		{
			UI_PaseMenu->OnClickedCancel_Button();

			// ボタンの位置を初期化
			ButtonCounter = 1;
			break;
		}
		case 3:
			UI_PaseMenu->OnClickedGameEnd_Button();
			break;

	}
}

void ABattleMapScript::InitializeButtonPosition()
{
	// プレイヤーコントローラーを取得
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	// ウィジェットブループリントのパスをセット
	FString path = "/Game/UMG/WBP_PaseMenu.WBP_PaseMenu_C";
	// アセットパスから UserWidgetClass を生成する
	WidgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*path)).LoadSynchronous();
	if (IsValid(WidgetClass))
	{
		// ポーズメニューをを生成
		UI_PaseMenu = Cast<UUI_PaseMenu>(CreateWidget(PlayerController, WidgetClass));
		// ポーズメニューがあったら
		if (UI_PaseMenu)
		{
			// ビューポートに表示する
			UI_PaseMenu->AddToViewport();
			UGameplayStatics::SetGamePaused(GetWorld(), true);
		}
		//ゲームパッドで動かせるように
		PlayerController->SetInputMode(FInputModeGameOnly());
	}

	// 最初 Back to Title を選択状態にする
	UButton* BackToTitleButton = UI_PaseMenu->GetBackToTitle_Button();
	if (BackToTitleButton)
	{
		BackToTitleButton->SetBackgroundColor(SelectedColor);
	}
}
