// Fill out your copyright notice in the Description page of Project Settings.


#include "GUNMANController.h"
#include "GUNMANCharacter.h"
#include "GUNMAN/UMG/UI_PaseMenu.h"
#include "GUNMAN/LevelScript/BattleMapScript.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GamePlayStatics.h"
#include "Components/Button.h"

void AGUNMANController::SetupInput()
{
	// 入力を有効にする
	EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	// ActionMappingsに設定したActionをバインドする
	InputComponent->BindAction("PauseMenu", IE_Pressed, this, &AGUNMANController::PressedActionPoseMenu);
}

void AGUNMANController::PressedActionPoseMenu()
{
	// ワールド情報があるか
	UWorld* World = GetWorld();
	if (World)
	{
		// レベルがあるか確認後、ポーズメニュー開いたときの最初のボタンの位置を初期化
		BattleMapRef = Cast<ABattleMapScript>(World->GetLevelScriptActor());
		if (BattleMapRef)
		{
			BattleMapRef->InitializeButtonPosition();
		}
	}
}

void AGUNMANController::BeginPlay()
{
	Super::BeginPlay();
	SetupInput();
}
