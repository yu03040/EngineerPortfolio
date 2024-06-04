// Fill out your copyright notice in the Description page of Project Settings.


#include "UITimeLimitWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetTextLibrary.h"

void UUITimeLimitWidget::NativeConstruct()
{
	// 制限時間は 120 秒
	Time = 120.0f;
}

bool UUITimeLimitWidget::Initialize()
{
	bool Success = Super::Initialize();

	if (Success == false)
	{
		return false;
	}

	TimeRemoving_TextBlock->TextDelegate.BindUFunction(this, "SetTextBlockRemainingTime");

	return true;
}

FText UUITimeLimitWidget::SetTextBlockRemainingTime()
{
	return UKismetTextLibrary::Conv_IntToText(UKismetMathLibrary::Round(Time));
}

