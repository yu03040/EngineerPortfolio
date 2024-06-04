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
	AAIEnemy* Enemy;
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	void OpenGameClearMap();

private:
	TSubclassOf<class UUserWidget> TitleWidget;

	UPROPERTY()
	class UUserWidget* TitleUserWidget = nullptr;

	/** UITimeLimitWidget の リファレンス */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	UUITimeLimitWidget* UITimeLimitRef;

	/** GUNMANCharacter の リファレンス */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	AGUNMANCharacter* PlayerRef;
};