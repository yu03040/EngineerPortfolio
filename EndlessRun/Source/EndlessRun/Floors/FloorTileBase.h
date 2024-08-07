// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "FloorTileBase.generated.h"

UENUM(BlueprintType)
enum class SpawnObj : uint8
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floor", meta = (AllowPrivateAccess = "true"))
	TArray<FTransform> SpawnPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floor", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UArrowComponent> AttachPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floor", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UArrowComponent> SpawnPointCenter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floor", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UArrowComponent> SpawnPointLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floor", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UArrowComponent> SpawnPointRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floor", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> CoinArea;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floor", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> EndTrigger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Delay", meta = (AllowPrivateAccess = "true"))
	FTimerHandle TimerHandle;

public:	
	// Sets default values for this actor's properties
	AFloorTileBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	//OnComponentBeginOverlap
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/*
	 * 生成する床の位置を調べる関数
	 * @return ワールド座標系の位置を Transform で返す
	 */
	UFUNCTION(BlueprintCallable, Category = "Floor")
	FTransform GetAttachPoint();

	// 障害物やコインを生成する位置を更新する関数
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Floor")
	void SetSpawnPoint();

	// 障害物を生成する関数
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Floor")
	void SpawnBlocker();

	// コインを生成する関数
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Floor")
	void SpawnCoins();
};
