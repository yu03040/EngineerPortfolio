// Fill out your copyright notice in the Description page of Project Settings.


#include "UIEnemy.h"
#include "Kismet/GamePlayStatics.h"
#include "Components/ProgressBar.h"

void UUIEnemy::NativeConstruct()
{
	// ユーザー定義の初期化処理を行う
	// プレイヤーキャラクター（ACharacter）を取得する
	Player = UGameplayStatics::GetPlayerCharacter(this->GetWorld(), 0);
	Construct();
}

void UUIEnemy::Construct()
{
	Super::Construct();

	// ここでUI要素の初期化や処理を行う
}

void UUIEnemy::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{

}
