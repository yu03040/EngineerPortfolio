// Copyright Epic Games, Inc. All Rights Reserved.

#include "GUNMANGameMode.h"
#include "GUNMANCharacter.h"
#include "GUNMANController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "UMG/UITitle.h"
#include "UObject/ConstructorHelpers.h"

AGUNMANGameMode::AGUNMANGameMode() : Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprint/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	PlayerControllerClass = AGUNMANController::StaticClass();
}

void AGUNMANGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 予め決めた制限時間から経過時間を引く
	UITimeLimitRef->Time -= DeltaTime;
	// 0 秒以下ならゲーム終了（ゲームオーバー）
	if (UITimeLimitRef->Time <= 0.0f)
	{
		UGameplayStatics::OpenLevel(this, "GameOverMap");
	}

	if (PlayerRef->GetKillCount() >= 25)
	{
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AGUNMANGameMode::OpenGameClearMap, 2.0f);
	}
}

void AGUNMANGameMode::BeginPlay()
{
	Super::BeginPlay();

	auto IsPlayer = IsValid(UGameplayStatics::GetPlayerCharacter(this->GetWorld(), 0));
	if (IsPlayer == true)
	{
		PlayerRef = Cast<AGUNMANCharacter>(UGameplayStatics::GetPlayerCharacter(this->GetWorld(), 0));
	}
}

void AGUNMANGameMode::OpenGameClearMap()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("GameClearMap")));
}
