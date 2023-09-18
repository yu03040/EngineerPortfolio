// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UCLASS()
class BATTERYCOLLECTOR_API APickup : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	//レベル上で拾われるメッシュ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* PickupMesh;

public:

	// 拾うメッシュを返す
	FORCEINLINE UStaticMeshComponent* GetMesh() const
	{
		return PickupMesh;
	}

	// このアイテムが拾えるかどうかの確認用
	UFUNCTION(BlueprintPure, Category = "Pickup")
		bool IsActive();

	// アイテムを拾えるかどうかを設定する
	UFUNCTION(BlueprintCallable, Category = "Pickup")
		void SetActive(bool NewPickupState);

	// アイテム回収時に呼び出される
	UFUNCTION(BlueprintNativeEvent, Category = "Pickup")
		void WasCollected();
	virtual void WasCollected_Implementation();

protected:

	// このアイテムが拾えるかどうか
	bool bIsActive;
};
