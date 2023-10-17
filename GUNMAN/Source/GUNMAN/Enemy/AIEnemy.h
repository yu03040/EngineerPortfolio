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
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Weapon;

	/** 敵が意思決定をするめための変数 */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = AI, meta = (AllowPrivateAccess = "true"))
	UAIPerceptionComponent* AIPerception;

	/** Healthバーを表示するための変数 */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Widget, meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* Widget;

public:
	// Sets default values for this character's properties
	AAIEnemy();

	/** プレイヤーかどうか判断する変数 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay)
	ACharacter* Player;

	/** Enemy のコントローラーを判断する変数 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay)
	AController* EnemyController;

	/** 受け取ったアクターを格納する変数 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	AActor* ReceivedActor;

	/** 敵の体力 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay)
	float Health = 30.0f;

	/** 倒した敵の数 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	int KillCount;

	/** 攻撃が当たったときのアニメーション */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	TArray<UAnimMontage*> HitAnimMontage;

	/** Healthバーを表示するための変数 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Widget, meta = (AllowPrivateAccess = "true"))
	UUIEnemy* TempWidget;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	/** 敵の生死 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay)
	bool IsAlive = true;

	/** タイマーハンドル */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FTimerHandle TargetLostTime;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Set Timer by Event のイベント */
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void TargetLost();
};
