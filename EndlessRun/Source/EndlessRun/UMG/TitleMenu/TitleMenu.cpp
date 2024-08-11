// Fill out your copyright notice in the Description page of Project Settings.


#include "EndlessRun/UMG/TitleMenu/TitleMenu.h"
#include "Animation/WidgetAnimation.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UTitleMenu::NativeConstruct()
{
	Super::NativeConstruct();

	// Button_StartのOnClickedに「OnButtonStartClicked」を関連づける
	Button_Start->OnClicked.AddUniqueDynamic(this, &UTitleMenu::OnButtonStartClicked);

	// Button_EndのOnClickedに「OnButtonEndClicked」を関連づける
	Button_End->OnClicked.AddUniqueDynamic(this, &UTitleMenu::OnButtonEndClicked);

	// ロゴアニメーションの再生
	UUserWidget::PlayAnimation(WidgetAnimation_LogoFrameIn);
}

void UTitleMenu::OnButtonStartClicked()
{
	OnGameStarted.Broadcast();
}

void UTitleMenu::OnButtonEndClicked()
{
	// ゲームを終了する
	UKismetSystemLibrary::QuitGame(this, NULL, EQuitPreference::Quit, false);
}