// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GUNMAN/GameModes/GUNMANGameModeBase.h"
#include "GameClearMode.generated.h"

/**
 * 
 */
UCLASS()
class GUNMAN_API AGameClearMode : public AGUNMANGameModeBase
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
	// クリア画面を表示する関数
	void DisplayGameClear();

};