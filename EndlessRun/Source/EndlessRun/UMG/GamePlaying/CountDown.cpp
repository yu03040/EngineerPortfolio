// Fill out your copyright notice in the Description page of Project Settings.


#include "EndlessRun/UMG/GamePlaying/CountDown.h"
#include "EndlessRun/GameModes/RunGameMode.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/TextBlock.h"
#include "HAL/PlatformProcess.h"
#include "TimerManager.h"

void UCountDown::NativeConstruct()
{
	Super::NativeConstruct();

	TObjectPtr<ACharacter> Player = UGameplayStatics::GetPlayerCharacter(this, 0);
	TObjectPtr<APlayerController> OwningPlayerController = GetOwningPlayer();

	Player->DisableInput(OwningPlayerController);

	CountTexts.Add(TextBlock_GO);
	CountTexts.Add(TextBlock_1);
	CountTexts.Add(TextBlock_2);
	CountTexts.Add(TextBlock_3);
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UCountDown::UpdateCountdown, 1.0f, true);
}

void UCountDown::SetTextBlockOpacity(UTextBlock* TextBlock, float Opacity)
{
	if (TextBlock)
    {
		FLinearColor TextColor = TextBlock->ColorAndOpacity.GetSpecifiedColor();
        TextColor.A = Opacity;
        TextBlock->SetColorAndOpacity(FSlateColor(TextColor));
    }
}

void UCountDown::UpdateCountdown()
{
	for (int i = 0; i < CountTexts.Num(); i++)
	{
		if (CountTexts[i])
		{
			// CountDownTime によって不透明度を更新する
			float Opacity = (i == CountDownTime) ? 1.0f : 0.0f;
			SetTextBlockOpacity(CountTexts[i], Opacity);
		}
	}

	if (CountDownTime < 0) // CountDownTime == 0 だと 「GO!!!」と同時に消してしまう
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

		TObjectPtr<ACharacter> Player = UGameplayStatics::GetPlayerCharacter(this, 0);
		TObjectPtr<APlayerController> OwningPlayerController = GetOwningPlayer();

		// 入力できるようになる
		Player->EnableInput(OwningPlayerController);

		// キャラクターの Tick を再開（走れるようになる）
		Player->SetActorTickEnabled(true);

		TObjectPtr<ARunGameMode> GameMode = Cast<ARunGameMode>(UGameplayStatics::GetGameMode(this));
		if (GameMode)
		{
			// ゲームモードの Tick を再開（走行距離を計算できるようになる）
			GameMode->SetActorTickEnabled(true);
		}

		// ウィジェットを削除する
		RemoveFromViewport();
	}

	CountDownTime--;
}
