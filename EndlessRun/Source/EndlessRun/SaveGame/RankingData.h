// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RankingData.generated.h"


USTRUCT(BlueprintType)
struct FRankingData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FloorTile", meta = (AllowPrivateAccess = "true"))
	int Score;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FloorTile", meta = (AllowPrivateAccess = "true"))
	float Distance;

	bool operator==(const FRankingData& Other) const
	{
		return Score == Other.Score && Distance == Other.Distance;
	}
};
