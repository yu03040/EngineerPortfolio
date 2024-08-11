// Fill out your copyright notice in the Description page of Project Settings.


#include "EndlessRun/UMG/PauseMenu/PauseMenu.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void UPauseMenu::NativeConstruct()
{
	Super::NativeConstruct();

	// マウスカーソルをオンにする
	TObjectPtr<APlayerController> Controller = UGameplayStatics::GetPlayerController(this, 0);
	Controller->bShowMouseCursor = true;

	// ゲームを一時停止する
	bool bPaused = true;
	UGameplayStatics::SetGamePaused(this, bPaused);

	// ゲームの入力を止める
	Controller->DisableInput(Controller);

	// UI の操作を可能にする
	UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(Controller, NULL, EMouseLockMode::DoNotLock, true, false);

	// Button_BackToTitleのOnClickedに「OnButtonBackToTitleClicked」を関連づける
	Button_BackToTitle->OnClicked.AddUniqueDynamic(this, &UPauseMenu::OnButtonBackToTitleClicked);

	// Button_CancelのOnClickedに「OnButtonCancelClicked」を関連づける
	Button_Cancel->OnClicked.AddUniqueDynamic(this, &UPauseMenu::OnButtonCancelClicked);

	// Button_QuitGameのOnClickedに「OnButtonQuitGameClicked」を関連づける
	Button_QuitGame->OnClicked.AddUniqueDynamic(this, &UPauseMenu::OnButtonQuitGameClicked);
}

void UPauseMenu::OnButtonBackToTitleClicked()
{
	UGameplayStatics::PlaySound2D(GetWorld(), ClickSound);

	// ポーズ中のため Delay を使用
	FLatentActionInfo LatentInfo;
	LatentInfo.Linkage = 0;
	LatentInfo.UUID = FMath::Rand();
	LatentInfo.ExecutionFunction = FName("BackTitle");
	LatentInfo.CallbackTarget = this;

	UKismetSystemLibrary::Delay(GetWorld(), 0.5f, LatentInfo);
}

void UPauseMenu::OnButtonCancelClicked()
{
	UGameplayStatics::PlaySound2D(GetWorld(), ClickSound);

	// ポーズ中のため Delay を使用
	FLatentActionInfo LatentInfo;
	LatentInfo.Linkage = 0;
	LatentInfo.UUID = FMath::Rand();
	LatentInfo.ExecutionFunction = FName("Cancel");
	LatentInfo.CallbackTarget = this;

	UKismetSystemLibrary::Delay(GetWorld(), 0.5f, LatentInfo);
}

void UPauseMenu::OnButtonQuitGameClicked()
{
	UGameplayStatics::PlaySound2D(GetWorld(), ClickSound);

	// ポーズ中のため Delay を使用
	FLatentActionInfo LatentInfo;
	LatentInfo.Linkage = 0;
	LatentInfo.UUID = FMath::Rand();
	LatentInfo.ExecutionFunction = FName("EndGame");
	LatentInfo.CallbackTarget = this;

	UKismetSystemLibrary::Delay(GetWorld(), 0.5f, LatentInfo);
}

void UPauseMenu::BackTitle()
{
	FName LevelName = TEXT("TitleMap");
	UGameplayStatics::OpenLevel(this, LevelName);
}

void UPauseMenu::Cancel()
{
	// ゲームの入力を再開する
	TObjectPtr<APlayerController> Controller = UGameplayStatics::GetPlayerController(this, 0);
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(Controller, false);

	RemoveFromParent();

	// ゲームを再開する
	bool bPaused = false;
	UGameplayStatics::SetGamePaused(this, bPaused);

	// マウスカーソルをオフにする
	Controller->bShowMouseCursor = false;
}

void UPauseMenu::EndGame()
{
	// ゲームを終了する
	UKismetSystemLibrary::QuitGame(this, NULL, EQuitPreference::Quit, false);
}
