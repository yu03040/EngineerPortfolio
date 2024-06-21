// Fill out your copyright notice in the Description page of Project Settings.


#include "UIGameOver.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GamePlayStatics.h"

void UUIGameOver::NativeConstruct()
{
	if (Continue_Button)
	{
		Continue_Button->OnClicked.AddDynamic(this, &UUIGameOver::OnClickedContinue_Button);
	}

	if (GameEnd_Button)
	{
		GameEnd_Button->OnClicked.AddDynamic(this, &UUIGameOver::OnClickedGameEnd_Button);

	}
}

void UUIGameOver::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
}


void UUIGameOver::OnClickedContinue_Button()
{
	// コンティニューする
	UGameplayStatics::OpenLevel(this, "TitleMap");
}

void UUIGameOver::OnClickedGameEnd_Button()
{
	// ゲーム終了
	UKismetSystemLibrary::QuitGame(this, NULL, EQuitPreference::Quit, false);
}