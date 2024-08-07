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

void UCountDown::CountDownEvent()
{
	// 3, 2, 1, GO まで繰り返す
	//if (0 <= Index && Index < CountTexts.Num() - 1)
	//{
	//	// 不透明にする

	//	// 1 秒後（Delay(0.5秒) + SetTimer(0.5秒)）に透明にする
	//	float DelayTime = 1.0f;
	//	//StartDelay(DelayTime, FName("SetTextTransparency"));

	//	Index++;
	//	StartDelay(DelayTime, FName("CountDownEvent"));
	//}
	//else if (Index == CountTexts.Num() - 1)
	//{
	//	TObjectPtr<ACharacter> Player = UGameplayStatics::GetPlayerCharacter(this, 0);
	//	TObjectPtr<APlayerController> OwningPlayerController = GetOwningPlayer();

	//	// 入力できるようになる
	//	Player->EnableInput(OwningPlayerController);

	//	// キャラクターの Tick を再開（走れるようになる）
	//	bool bEnabled = true;
	//	Player->SetActorTickEnabled(bEnabled);

	//	TObjectPtr<ARunGameMode> GameMode = Cast<ARunGameMode>(UGameplayStatics::GetGameMode(this));
	//	if (GameMode)
	//	{
	//		// ゲームモードの Tick を再開（走行距離を計算できるようになる）
	//		GameMode->SetActorTickEnabled(bEnabled);
	//	}
	//}
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
			float Opacity = (i == CountDownTime) ? 1.0f : 0.0f;
			SetTextBlockOpacity(CountTexts[i], Opacity);
		}
	}

	if (CountDownTime == 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

		TObjectPtr<ACharacter> Player = UGameplayStatics::GetPlayerCharacter(this, 0);
		TObjectPtr<APlayerController> OwningPlayerController = GetOwningPlayer();

		// 入力できるようになる
		Player->EnableInput(OwningPlayerController);

		// キャラクターの Tick を再開（走れるようになる）
		bool bEnabled = true;
		Player->SetActorTickEnabled(bEnabled);

		TObjectPtr<ARunGameMode> GameMode = Cast<ARunGameMode>(UGameplayStatics::GetGameMode(this));
		if (GameMode)
		{
			// ゲームモードの Tick を再開（走行距離を計算できるようになる）
			GameMode->SetActorTickEnabled(bEnabled);
		}
	}

	CountDownTime--;
}

void UCountDown::StartDelay(float DelayTime, FName FuncName)
{
	FLatentActionInfo LatentInfo;
	LatentInfo.Linkage = 0;
	LatentInfo.UUID = FMath::Rand();
	LatentInfo.ExecutionFunction = FuncName;
	LatentInfo.CallbackTarget = this;

	UKismetSystemLibrary::Delay(this, DelayTime, LatentInfo);
}
