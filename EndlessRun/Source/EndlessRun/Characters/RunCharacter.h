// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "RunCharacter.generated.h"

class UInputAction;

UCLASS()
class ENDLESSRUN_API ARunCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> PauseAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> TurnLeftAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> TurnRightAction;

	// 曲がるときに欲しい角度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FRotator DesiredRotation;

	// 曲がれる場合は true, そうでない場合は false
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bCanTurn;

	// タイマーを一意に識別するためのハンドル
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FTimerHandle TimerHandle;

	// 爆発エフェクト
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VFX, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UParticleSystem> Emitter;

	// 爆発音
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VFX, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundBase> ExplosionSound;

protected:
	// ゲーム開始時またはスポーン時にコールされる
	virtual void BeginPlay() override;

public:
	// 死んだ場合は true, そうでない場合は false
	bool bIsDead;

	// エフェクトを発生した場合は true, そうでない場合は false
	bool bIsEffect;

public:
	// このキャラクターのプロパティのデフォルト値を設定する（コンストラクター）
	ARunCharacter();
	
	// フレームごとに呼び出される
	virtual void Tick(float DeltaTime) override;

	// 入力に機能をバインドするために呼び出される
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	/**
	*  カウントダウンを表示する関数
	*  @param PlayerController プレイヤーコントローラーの参照
	*/
	void DisplayCountDown(TObjectPtr<APlayerController>& PlayerController);

	// ポーズする関数
	UFUNCTION()
	void PauseInput();

	// ジャンプを開始する関数
	UFUNCTION()
	void StartJump();

	// ジャンプを終了する関数
	UFUNCTION()
	void StopJump();

	// 左右に移動する関数
	UFUNCTION()
	void MovementInput(const FInputActionValue& Value);

	// 左に曲がる関数
	UFUNCTION()
	void TurnLeftMovement();

	// 右に曲がる関数
	UFUNCTION()
	void TurnRightMovement();

	// プレイヤーの向きを変更する関数
	UFUNCTION()
	void TurnCorner();

	// プレイヤーが死んだとき呼ばれる関数
	UFUNCTION(BlueprintCallable)
	void Death();
};
