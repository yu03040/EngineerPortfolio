// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blocker.generated.h"

class UBoxComponent;
class UNiagaraComponent;

UCLASS()
class ENDLESSRUN_API ABlocker : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blocker", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	// Box Trigger
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blocker", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> Box;

	// ビジュアルエフェクト
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blocker", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraComponent> Niagara;

protected:
	// ゲーム開始時またはスポーン時にコールされる
	virtual void BeginPlay() override;

public:
	// この障害物のプロパティのデフォルト値を設定する（コンストラクター）
	ABlocker();

	// フレームごとに呼び出される
	virtual void Tick(float DeltaTime) override;

public:
	// Box のオーバーラップイベント
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
