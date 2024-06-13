// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIControllerInterface.h"
#include "AIEnemyController.generated.h"

/**
 *
 */
UCLASS()
class GUNMAN_API AAIEnemyController : public AAIController, public IEnemyAIControllerInterface
{
	GENERATED_BODY()

public:
	/** 受け取ったアクターの情報を更新する関数 */
	void UpdateTargetActorKey_Implementation(AActor* TargetActor) override;

	void UpdateHasLineOfSightKey_Implementation(bool HasLineOfSight) override;

	/** プレイヤーかどうか判断する変数 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay)
	TObjectPtr<ACharacter> Player;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
