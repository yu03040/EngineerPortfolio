// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GUNMANController.generated.h"

/**
 * 
 */
UCLASS()
class GUNMAN_API AGUNMANController : public APlayerController
{
	GENERATED_BODY()
	
private:
	// Input設定
	void SetupInput();

	// Input Event イベントハンドラー関数
	void PressedActionPoseMenu();

	UPROPERTY()
	class UUI_PaseMenu* UI_PaseMenu;

	/* ブループリントのウィジェットクラス */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> WidgetClass;

	/* BattleMap の Ref */
	class ABattleMapScript* BattleMapRef;
protected:
	virtual void BeginPlay() override;
};
