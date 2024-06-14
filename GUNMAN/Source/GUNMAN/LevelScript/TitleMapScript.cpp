// Fill out your copyright notice in the Description page of Project Settings.


#include "GUNMAN/LevelScript/TitleMapScript.h"
#include "GUNMAN/UMG/UITitle.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

ATitleMapScript::ATitleMapScript()
{
	UI_Title = NULL;
	MaxButtonCounter = 3;
	InvalidButtonIndex = 4;
}

void ATitleMapScript::BeginPlay()
{
	Super::BeginPlay();

	// プレイヤーコントローラーを取得
	TObjectPtr<APlayerController> PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	// ウィジェットブループリントのパスをセット
	FString path = "/Game/UMG/WBP_Title.WBP_Title_C";
	// アセットパスから UserWidgetClass を生成する
	WidgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*path)).LoadSynchronous();
	// タイトルのウィジェットを作成
	if (IsValid(WidgetClass))
	{
		UI_Title = Cast<UUITitle>(CreateWidget(PlayerController, WidgetClass));

		if (UI_Title)
		{
			UI_Title->AddToViewport();
		}
	}

	//ゲームパッドで動かせるように
	PlayerController->SetInputMode(FInputModeGameOnly());

	// 最初 Game Start を選択状態にする
	TObjectPtr<UButton> StartButton = UI_Title->GetGameStart_Button();
	if (StartButton)
	{
		StartButton->SetBackgroundColor(SelectedColor);
	}
}

void ATitleMapScript::ChangeButtonColor()
{
	// 非選択状態の色をそれぞれ白色に
	TObjectPtr<UButton> StartButton = UI_Title->GetGameStart_Button();
	if (StartButton)
	{
		StartButton->SetBackgroundColor(FLinearColor::White);
	}

	TObjectPtr<UButton> EndButton = UI_Title->GetGameEnd_Button();
	if (EndButton)
	{
		EndButton->SetBackgroundColor(FLinearColor::White);
	}

	TObjectPtr<UButton> StartTutorialButton = UI_Title->GetGameStartExplaination_Button();
	if (StartTutorialButton)
	{
		StartTutorialButton->SetBackgroundColor(FLinearColor::White);
	}

	// 選択状態のボタンの色をそれぞれ変える
	switch (ButtonCounter)
	{
		case 1:
			StartButton->SetBackgroundColor(SelectedColor);
			break;
		case 2:
			EndButton->SetBackgroundColor(SelectedColor);
			break;
		case 3:
			StartTutorialButton->SetBackgroundColor(SelectedColor);
			break;
	}
}

void ATitleMapScript::UpdateOutputButton()
{
	// 操作説明のキャンバスにいるか
	if (HasMovedToInstructions)
	{
		// 操作説明のキャンバスを閉じる
		UI_Title->OnClickedGameEndExplaination_Button();
		HasMovedToInstructions = false;
	}
	else
	{
		switch (ButtonCounter)
		{
			case 1:
				UI_Title->OnClickedGameStart_Button();
				break;
			case 2:
				UI_Title->OnClickedGameEnd_Button();
				break;
			case 3:
				// 操作説明のキャンバスを閉じるボタンの色を変える
				UI_Title->OnClickedGameStartExplaination_Button();
				TObjectPtr<UButton> EndTutorialButton = UI_Title->GetGameEndExplaination_Button();
				EndTutorialButton->SetBackgroundColor(SelectedColor);
				HasMovedToInstructions = true;
				break;
		}
	}
}