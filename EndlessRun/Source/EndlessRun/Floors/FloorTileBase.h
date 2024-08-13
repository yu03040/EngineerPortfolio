// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "FloorTileBase.generated.h"

UENUM(BlueprintType)
enum class ESpawnObj : uint8
{
	Blocker,
	Coins,
};

UCLASS()
class ENDLESSRUN_API AFloorTileBase : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floor", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	// 障害物が発生する場所の配列
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floor", meta = (AllowPrivateAccess = "true"))
	TArray<FTransform> SpawnPoints;

	// 次の床の発生場所
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floor", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UArrowComponent> AttachPoint;

	// 障害物が発生する場所：真ん中
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floor", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UArrowComponent> SpawnPointCenter;

	// 障害物が発生する場所：左
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floor", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UArrowComponent> SpawnPointLeft;

	// 障害物が発生する場所：右
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floor", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UArrowComponent> SpawnPointRight;

	// コインが発生する場所
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floor", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> CoinArea;

	// 床を通りすぎるときのトリガー
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floor", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> EndTrigger;

	// タイマーを一意に識別するためのハンドル
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Delay", meta = (AllowPrivateAccess = "true"))
	FTimerHandle TimerHandle;

public:	
	// この床のプロパティのデフォルト値を設定する（コンストラクター）
	AFloorTileBase();

protected:
	// ゲーム開始時またはスポーン時にコールされる
	virtual void BeginPlay() override;
	
	// EndTrigger のオーバーラップイベント
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// フレームごとに呼び出される
	virtual void Tick(float DeltaTime) override;

	/*
	 * 生成する床の位置を調べる関数
	 * @return ワールド座標系の位置を Transform で返す
	 */
	UFUNCTION(BlueprintCallable, Category = "Floor")
	FTransform GetAttachPoint() { return AttachPoint->GetComponentTransform(); }

	// 障害物やコインを生成する位置を更新する関数
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Floor")
	void SetSpawnPoint();

	// 障害物を生成する関数
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Floor")
	void SpawnBlocker();

	// コインを生成する関数
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Floor")
	void SpawnCoins();

	// 床を削除する関数
	UFUNCTION()
	void DestroyActor();
};
