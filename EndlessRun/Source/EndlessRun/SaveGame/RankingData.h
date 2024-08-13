// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RankingData.generated.h"


USTRUCT(BlueprintType)
struct FRankingData
{
	GENERATED_BODY()

public:
	// 取ったコインの数
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FloorTile", meta = (AllowPrivateAccess = "true"))
	int Score;

	// 走った距離
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FloorTile", meta = (AllowPrivateAccess = "true"))
	float Distance;
};
