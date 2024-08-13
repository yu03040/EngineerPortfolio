// Fill out your copyright notice in the Description page of Project Settings.


#include "EndlessRun/UMG/TitleMenu/TitleMenu.h"
#include "Animation/WidgetAnimation.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Sound/SoundBase.h"
#include "TimerManager.h"

void UTitleMenu::NativeConstruct()
{
	Super::NativeConstruct();

	// Button_Start の OnClicked に「OnButtonStartClicked」を関連づける
	Button_Start->OnClicked.AddUniqueDynamic(this, &UTitleMenu::OnButtonStartClicked);

	// Button_End の OnClicked に「OnButtonEndClicked」を関連づける
	Button_End->OnClicked.AddUniqueDynamic(this, &UTitleMenu::OnButtonEndClicked);

	// ロゴアニメーションの再生
	UUserWidget::PlayAnimation(WidgetAnimation_LogoFrameIn);
}

void UTitleMenu::OnButtonStartClicked()
{
	UGameplayStatics::PlaySound2D(GetWorld(), ClickSound);
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UTitleMenu::StartGame, 0.5f, false);
}

void UTitleMenu::OnButtonEndClicked()
{
	UGameplayStatics::PlaySound2D(GetWorld(), ClickSound);
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UTitleMenu::EndGame, 0.5f, false);
}

void UTitleMenu::StartGame()
{
	// 呼び出し
	OnGameStarted.Broadcast();
}

void UTitleMenu::EndGame()
{
	// ゲームを終了する
	UKismetSystemLibrary::QuitGame(this, NULL, EQuitPreference::Quit, false);
}