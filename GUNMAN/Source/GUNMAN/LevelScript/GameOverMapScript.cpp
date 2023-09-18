// Fill out your copyright notice in the Description page of Project Settings.


#include "GUNMAN/LevelScript/GameOverMapScript.h"
#include "GUNMAN/UMG/UIGameOver.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"


AGameOverMapScript::AGameOverMapScript()
{
	UI_GameOver = NULL;
	MaxButtonCounter = 2;
	InvalidButtonIndex = 3;
}

void AGameOverMapScript::BeginPlay()
{
	Super::BeginPlay();

	// プレイヤーコントローラーを取得
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	// ウィジェットブループリントのパスをセット
	FString path = "/Game/UMG/WBP_GameOver.WBP_GameOver_C";
	// アセットパスから UserWidgetClass を生成する
	WidgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*path)).LoadSynchronous();
	// タイトルのウィジェットを作成
	if (IsValid(WidgetClass))
	{
		UI_GameOver = Cast<UUIGameOver>(CreateWidget(PlayerController, WidgetClass));

		if (UI_GameOver)
		{
			UI_GameOver->AddToViewport();
		}
	}

	//ゲームパッドで動かせるように
	PlayerController->SetInputMode(FInputModeGameOnly());

	// 最初 Continue を選択状態にする
	UButton* ContinueButton = UI_GameOver->GetContinue_Button();
	if (ContinueButton)
	{
		ContinueButton->SetBackgroundColor(SelectedColor);
	}
}

void AGameOverMapScript::ChangeButtonColor()
{
	// 非選択状態の色をそれぞれ白色に
	UButton* ContinueButton = UI_GameOver->GetContinue_Button();
	if (ContinueButton)
	{
		ContinueButton->SetBackgroundColor(FLinearColor::White);
	}

	UButton* EndButton = UI_GameOver->GetGameEnd_Button();
	if (EndButton)
	{
		EndButton->SetBackgroundColor(FLinearColor::White);
	}

	// 選択状態のボタンの色をそれぞれ変える
	switch (ButtonCounter)
	{
		case 1:
			ContinueButton->SetBackgroundColor(SelectedColor);
			break;
		case 2:
			EndButton->SetBackgroundColor(SelectedColor);
			break;
	}
}

void AGameOverMapScript::UpdateOutputButton()
{
	switch (ButtonCounter)
	{
		case 1:
			UI_GameOver->OnClickedContinue_Button();
			break;
		case 2:
			UI_GameOver->OnClickedGameEnd_Button();
			break;
	}
}
