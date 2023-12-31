// Fill out your copyright notice in the Description page of Project Settings.


#include "../UMG/UI_PaseMenu.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GamePlayStatics.h"

void UUI_PaseMenu::NativeConstruct()
{
}

void UUI_PaseMenu::OnClickedBackToTitle_Button()
{
	// タイトルへ戻る
	UGameplayStatics::OpenLevel(this, "TitleMap");
	//UGameplayStatics::SetGamePaused(GetWorld(), false);
}

void UUI_PaseMenu::OnClickedCancel_Button()
{
	RemoveFromParent();
	UGameplayStatics::SetGamePaused(GetWorld(), false);
}

void UUI_PaseMenu::OnClickedGameEnd_Button()
{
	// ゲーム終了
	UKismetSystemLibrary::QuitGame(this, NULL, EQuitPreference::Quit, false);
}

void UUI_PaseMenu::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
}

UButton* UUI_PaseMenu::GetBackToTitle_Button() const
{
	return BackToTitle_Button;
}

UButton* UUI_PaseMenu::GetCancel_Button() const
{
	return Cancel_Button;
}

UButton* UUI_PaseMenu::GetGameEnd_Button() const
{
	return GameEnd_Button;
}
