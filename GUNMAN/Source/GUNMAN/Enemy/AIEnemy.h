// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/WidgetComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "GUNMAN/UMG/UIEnemy.h"
#include "AIEnemy.generated.h"

UCLASS()
class GUNMAN_API AAIEnemy : public ACharacter
{
	GENERATED_BODY()

	/** ThirdPerson の武器 */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Enemy, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Weapon;

	/** 敵が意思決定をするめための変数 */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Enemy, meta = (AllowPrivateAccess = "true"))
	UAIPerceptionComponent* AIPerception;

	/** Healthバーを表示するための変数 */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Enemy, meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* Widget;

	/** プレイヤーかどうか判断する変数 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Enemy, meta = (AllowPrivateAccess = "true"))
	ACharacter* Player;

	/** Enemy のコントローラーを判断する変数 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Enemy, meta = (AllowPrivateAccess = "true"))
	AController* EnemyController;

	/** 受け取ったアクターを格納する変数 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enemy, meta = (AllowPrivateAccess = "true"))
	AActor* ReceivedActor;

	/** 攻撃が当たったときのアニメーション */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enemy, meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> HitAnimMontage;

	/** Healthバーを表示するための変数 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Enemy, meta = (AllowPrivateAccess = "true"))
	UUIEnemy* EnemyWidget;

public:
	// Sets default values for this character's properties
	AAIEnemy();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/* 敵の体力のgetter */
	UFUNCTION(BlueprintCallable, Category = Enemy)
	float GetHealth();

	/* 敵の体力のsetter */
	UFUNCTION(BlueprintCallable, Category = Enemy)
	void SetHealth(float health);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/* 被ダメージ処理関数 */ 
	UFUNCTION()
	void HandleAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	/** Set Timer by Event のイベント */
	UFUNCTION(BlueprintCallable, Category = Enemy)
	void TargetLost();

protected:

	/** 敵の生死 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Enemy)
	bool IsAlive = true;

	/** タイマーハンドル */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Enemy)
	FTimerHandle TH_TargetLost;

	/** 敵の体力 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Enemy)
	float Health = 30.0f;

	/** 敵の体力 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Enemy)
	float EnemyATK = 10.0f;
};
