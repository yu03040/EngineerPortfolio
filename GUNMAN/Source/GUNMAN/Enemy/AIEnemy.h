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

	/** プレイヤーかどうか判断する変数 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	ACharacter* Player;

	/** Enemy のコントローラーを判断する変数 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	AController* EnemyController;

	/** 受け取ったアクターを格納する変数 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	AActor* ReceivedActor;

	/** 攻撃が当たったときのアニメーション */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> HitAnimMontage;

	/** Healthバーを表示するための変数 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Widget, meta = (AllowPrivateAccess = "true"))
	UUIEnemy* TempWidget;

public:
	// Sets default values for this character's properties
	AAIEnemy();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** 敵の体力 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay)
	float Health = 30.0f;

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

	/** Set Timer by Event のイベント */
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void TargetLost();
};
