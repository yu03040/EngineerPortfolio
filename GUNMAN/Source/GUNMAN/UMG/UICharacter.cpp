// Fill out your copyright notice in the Description page of Project Settings.


#include "UICharacter.h"
#include "Kismet/GamePlayStatics.h"
#include "Components/ProgressBar.h"

void UUICharacter::NativeConstruct()
{
	// ユーザー定義の初期化処理を行う
	// プレイヤーキャラクター（ACharacter）を取得する
	Player = UGameplayStatics::GetPlayerCharacter(this->GetWorld(), 0);
	// 制限時間は 120 秒
	TimeLimit = 120.0f;
	Construct();
}

void UUICharacter::Construct()
{
	Super::Construct();

	// ここでUI要素の初期化や処理を行う
}

void UUICharacter::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}
