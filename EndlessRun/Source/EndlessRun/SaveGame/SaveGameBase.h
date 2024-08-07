// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "RankingData.h"
#include "SaveGameBase.generated.h"

/**
 *
 */
UCLASS()
class ENDLESSRUN_API USaveGameBase : public USaveGame
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveGame", meta = (AllowPrivateAccess = "true"))
	TArray<FRankingData> RankingData;

public:
	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	TArray<FRankingData> GetRankingData() { return RankingData; }
};
