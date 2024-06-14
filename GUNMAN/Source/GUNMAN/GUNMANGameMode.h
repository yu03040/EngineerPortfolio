// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Enemy/AIEnemy.h"
#include "GUNMANCharacter.h"
#include "UMG/UITimeLimitWidget.h"
#include "GUNMANGameMode.generated.h"

UCLASS(minimalapi)
class AGUNMANGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGUNMANGameMode();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	void OpenGameClearMap();

	UPROPERTY()
	TObjectPtr<AAIEnemy> EnemyRef;

private:
	TSubclassOf<class UUserWidget> TitleWidget;

	UPROPERTY()
	TObjectPtr<class UUserWidget> TitleUserWidget = nullptr;

	/** ゲームクリアに必要な倒した数 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	int GameClearKillCount = 10;

	/** ゲームオーバーの時間 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	float GameOverTime = 0.0f;

	/** ゲームクリア後の待機時間 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	float GameClearWaitingTime = 2.0f;

	/** UITimeLimitWidget の リファレンス */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UUITimeLimitWidget> UITimeLimitRef;

	/** GUNMANCharacter の リファレンス */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AGUNMANCharacter> PlayerRef;

	/* ブループリントのウィジェットクラス */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> WidgetClass;
};