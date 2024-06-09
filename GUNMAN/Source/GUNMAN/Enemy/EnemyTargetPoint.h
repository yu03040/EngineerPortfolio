// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "GUNMAN/Enemy/AIEnemyController.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Engine/World.h"
#include "EnemyTargetPoint.generated.h"

/**
 * 
 */
UCLASS()
class GUNMAN_API AEnemyTargetPoint : public ATargetPoint
{
	GENERATED_BODY()
	
	/* 出現させる敵カウンター */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Enemy, meta = (AllowPrivateAccess = "true"))
	int EnemyCounter;

	/* 出現させる敵の最大数 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Enemy, meta = (AllowPrivateAccess = "true"))
	int MaxEnemyCount;

	/* 敵のブループリント */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Enemy, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<APawn> BPEnemyPawnClass;

	/* 敵が出現する間隔 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Enemy, meta = (AllowPrivateAccess = "true"))
	float SpawningIntervalTime;

	/* 出現する最小間隔 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Enemy, meta = (AllowPrivateAccess = "true"))
	float MinIntervalTime;

	/* 出現する最大間隔 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Enemy, meta = (AllowPrivateAccess = "true"))
	float MaxIntervalTime;

public:
	AEnemyTargetPoint();

	/* n秒待機する関数 */
	void StartDelay(float DelayTime, UWorld* World, TSubclassOf<APawn> PawnClass, const FVector& Location, const FRotator& Rotation);

	/* 敵を指定数スポーンする関数 */
	void SpawnEnemy();

	/* 敵にAIを付けスポーンさせる関数 */
	UFUNCTION(BlueprintCallable, Category = "EnemyTagetPoint")
	void SpawnAIClass(UWorld* World, TSubclassOf<APawn> PawnClass, const FVector& Location, const FRotator& Rotation);

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	FTimerHandle TimerHandle;
};
