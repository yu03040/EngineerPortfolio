// Fill out your copyright notice in the Description page of Project Settings.


#include "UICharacter.h"
#include "Kismet/GamePlayStatics.h"
#include "Components/ProgressBar.h"

void UUICharacter::NativeConstruct()
{
	// プレイヤーキャラクター（ACharacter）を取得する
	Player = UGameplayStatics::GetPlayerCharacter(this->GetWorld(), 0);
}

void UUICharacter::Construct()
{
	Super::Construct();
}

void UUICharacter::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}
