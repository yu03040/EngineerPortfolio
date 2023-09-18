// Fill out your copyright notice in the Description page of Project Settings.


#include "GUNMAN/LevelScript/GameClearMapScript.h"
#include "GUNMAN/UMG/UIGameClear.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

AGameClearMapScript::AGameClearMapScript()
{
	UI_GameClear = NULL;
	MaxButtonCounter = 2;
	InvalidButtonIndex = 3;
}

void AGameClearMapScript::BeginPlay()
{
	Super::BeginPlay();

	// プレイヤーコントローラーを取得
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	// ウィジェットブループリントのパスをセット
	FString path = "/Game/UMG/WBP_GameClear.WBP_GameClear_C";
	// アセットパスから UserWidgetClass を生成する
	WidgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*path)).LoadSynchronous();
	// タイトルのウィジェットを作成
	if (IsValid(WidgetClass))
	{
		UI_GameClear = Cast<UUIGameClear>(CreateWidget(PlayerController, WidgetClass));

		if (UI_GameClear)
		{
			UI_GameClear->AddToViewport();
		}
	}

	//ゲームパッドで動かせるように
	PlayerController->SetInputMode(FInputModeGameOnly());

	// 最初 Continue を選択状態にする
	UButton* ContinueButton = UI_GameClear->GetContinue_Button();
	if (ContinueButton)
	{
		ContinueButton->SetBackgroundColor(SelectedColor);
	}
}

void AGameClearMapScript::ChangeButtonColor()
{
	// 非選択状態の色をそれぞれ白色に
	UButton* ContinueButton = UI_GameClear->GetContinue_Button();
	if (ContinueButton)
	{
		ContinueButton->SetBackgroundColor(FLinearColor::White);
	}

	UButton* EndButton = UI_GameClear->GetGameEnd_Button();
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

void AGameClearMapScript::UpdateOutputButton()
{

	switch (ButtonCounter)
	{
		case 1:
			UI_GameClear->OnClickedContinue_Button();
			break;
		case 2:
			UI_GameClear->OnClickedGameEnd_Button();
			break;
	}

}
