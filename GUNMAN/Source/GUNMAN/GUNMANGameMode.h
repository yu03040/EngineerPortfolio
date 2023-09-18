// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Enemy/AIEnemy.h"
#include "GUNMANGameMode.generated.h"

UCLASS(minimalapi)
class AGUNMANGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGUNMANGameMode();
	AAIEnemy* Enemy;
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	TSubclassOf<class UUserWidget> TitleWidget;

	UPROPERTY()
	class UUserWidget* TitleUserWidget = nullptr;
};



