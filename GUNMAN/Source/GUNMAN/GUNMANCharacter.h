// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ArmedWeapon/WeaponStructure.h"
#include "Components/TimelineComponent.h"
#include "../../../../../../../Program Files/Epic Games/UE_5.3/Engine/Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h"
#include "GameFramework/Character.h"
#include "UMG/UICharacter.h"
#include "UMG/UIGunSight.h"
#include "ArmedWeapon/AnimationInterface.h"
#include "ArmedWeapon/WeaponInterface.h"
#include "GUNMANCharacter.generated.h"


UCLASS(config = Game)
class AGUNMANCharacter : public ACharacter, public IAnimationInterface, public IWeaponInterface
{
	GENERATED_BODY()

private:
	/* カメラをキャラクターの背後に配置するカメラブーム */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> ThirdPersonCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FirstPersonCamera;

	/* 左右視点移動の割合（単位：deg/sec）、その他のスケーリングが最終的なレートに影響する場合もある。 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float BaseTurnRate;

	/* 上下視点移動の割合（単位：deg/sec）、その他のスケーリングが最終的なレートに影響する場合もある。 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float BaseLookUpRate;

	/* ポーンメッシュ：1人称視点（腕） */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = FirstPerson, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Mesh1P;

	/* 銃メッシュ：1人称視点（自分しか見えない） */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = FirstPerson, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> FP_Gun;

	/* 銃のメッシュ上で弾がスポーンする場所 */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = FirstPerson, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> FP_MuzzleLocation;

	/* FirstPerson の武器の攻撃力 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FirstPerson, meta = (AllowPrivateAccess = "true"))
	float FP_WeaponATK = 5.0f;

	/* FirstPerson の武器がスポーンする銃弾 */
	UPROPERTY(EditDefaultsOnly, Category = FirstPerson, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AfirstpersonProjectProjectile> ProjectileClass;

	/* 銃口とキャラクターの位置のオフセット */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FirstPerson, meta = (AllowPrivateAccess = "true"))
	FVector GunOffset;

	/* 発射アニメーション */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FirstPerson, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> FireAnimation;

	/* ThirdPerson の武器 */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = ThirdPerson, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Weapon;

	/* 玉が着弾したときのエフェクト */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ThirdPerson, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UParticleSystem> WeaponEmitter;

	/* ThirdPerson の武器の配列 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ThirdPerson, meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<USkeletalMeshComponent>> WeaponMeshes;

	/* ThirdPerson の武器の index */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ThirdPerson, meta = (AllowPrivateAccess = "true"))
	int WeaponNumber;

	/* ThirdPerson の武器の攻撃力 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ThirdPerson, meta = (AllowPrivateAccess = "true"))
	float TP_WeaponATK;

	/* ThirdPerson のライフルの攻撃力 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ThirdPerson, meta = (AllowPrivateAccess = "true"))
	float TP_RifleATK = 5.0f;

	/* ThirdPerson のショットガンの攻撃力 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ThirdPerson, meta = (AllowPrivateAccess = "true"))
	float TP_ShotgunATK = 15.0f;

	/* ThirdPerson のピストルの攻撃力 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ThirdPerson, meta = (AllowPrivateAccess = "true"))
	float TP_PistolATK = 10.0f;

	/* ThirdPerson の武器のカウント */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ThirdPerson, meta = (AllowPrivateAccess = "true"))
	int WeaponNumberCounter;

	/* 装備している武器のメッシュ */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ThirdPerson, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> EquippedWeapon;

	/* ThirdPerson のアニメーションインスタンスをセットする */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ThirdPerson, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimInstance> TPMeshAnimInstance;

	/* 装備した武器の情報 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = ThirdPerson, meta = (AllowPrivateAccess = "true"))
	FWeaponStructure EquippedWeaponInformation;

	/* 武器を構えているか */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ThirdPerson, meta = (AllowPrivateAccess = "true"))
	bool IsAiming;

	/* 発射サウンドエフェクト */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FPAndTP, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundBase> FireSound;

	/* フリップフロップのための変数 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FPAndTP, meta = (AllowPrivateAccess = "true"))
	bool bIsFlipped = true;

	/* 武器を持っているか？ */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FPAndTP, meta = (AllowPrivateAccess = "true"))
	bool HasWeapon;

	/* FirstPerson か？ */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FPAndTP, meta = (AllowPrivateAccess = "true"))
	bool isFP = false;

	/* 攻撃できるか？ */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = FPAndTP, meta = (AllowPrivateAccess = "true"))
	bool CanAttack = true;

	/* 攻撃のタイマーハンドル */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FPAndTP, meta = (AllowPrivateAccess = "true"))
	FTimerHandle FireTimerHandle;

	/* 連射間隔 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = FPAndTP, meta = (AllowPrivateAccess = "true"))
	float FiringInterval = 0.1f;

	/* ジャンプボタンが押されたかどうか */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GamePlay, meta = (AllowPrivateAccess = "true"))
	bool JumpButtonDown = false;

	/* プレイヤーの体力 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GamePlay, meta = (AllowPrivateAccess = "true"))
	float MaxHealth = 1000.0f;

	/* 現在のプレイヤ―の体力 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GamePlay, meta = (AllowPrivateAccess = "true"))
	float CurrentHealth = MaxHealth;

	/* 死んだプレイヤ―の体力 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GamePlay, meta = (AllowPrivateAccess = "true"))
	float DeadHealth = 0.0f;

	/* 倒した敵の数 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GamePlay, meta = (AllowPrivateAccess = "true"))
	int KillCount = 0;

	/* BattleMapScript の参照 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GameMap, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class ABattleMapScript> BattleMapRef;

	/* キャラクターUI の参照 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UI, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UUICharacter> UICharacterRef;

	/* キャラクターUI の WBP */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> CharacterWidgetClass;

	/* キャラクターUIの WBP のパス */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI, meta = (AllowPrivateAccess = "true"))
	FString CharacterWidgetPath;

	/* 照準の UI の参照 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UI, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UUIGunSight> UIGunSightRef;

	/* 照準の WBP */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> GunSightWidgetClass;

	/* 照準の WBP のパス */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI, meta = (AllowPrivateAccess = "true"))
	FString GunSightWidgetPath;

	/* Jump Input */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	/* Fire Input */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> FireAction;

	/* Toggle Input */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ToggleAction;

	/* Ready Gun Input */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ReadyGunAction;

	/* Run Input */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> RunAction;

	/* Pause Menu Input */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> PauseMenuAction;

	/* Move Forward Input */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveForwardAction;

	/* Move Right Input */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveRightAction;

	/* Turn Input */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LookAction;

	/* プレイヤ―の速さの最小値 */
	UPROPERTY()
	float StartSpeed = 300.0f;

	/* プレイヤ―の速さの最大値 */
	UPROPERTY()
	float EndSpeed = 600.0f;

protected:
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* 攻撃開始 */
	void StartFire();

	/* 攻撃（呼び出し） */
	void FiringEvent();

	/* 攻撃終了（ボタンを離すと呼ばれなくなる） */
	void StopFire();

	void FireState_Implementation(bool CanATK) override;

	void AttachWeapon_Implementation(USkeletalMeshComponent* WeaponMesh, FName AttachSoketName) override;

	/* TPS と FPS の視点を切り替える */
	void ToggleBetweenTPSAndFPS();

	/* 銃を構える（開始） */
	void StartReadyGun();
	
	/* 
	 * 銃を構える過程
	 * @param bIsAiming 銃を構えているか？
	 * @param ArmLength カメラのアームの長さ
	 * @param bOrientRotationToMovement キャラクターの移動方向に応じて自動的にキャラクターの向きを調整するか？
	 * @param bYawRotation キャラクターがコントローラーの回転（Yaw）に従って回転するか？
	 * @param CameraBoomLocation カメラブームの新しい位置
	 * @param CameraBoomRotation カメラブームの新しい角度
	 */
	void GunPreparationProcess(bool bIsAiming, float ArmLength, bool bOrientRotationToMovement, bool bYawRotation, FVector CameraBoomLocation, FRotator CameraBoomRotation);

	/* 銃を構える（終了） */
	void StopReadyGun();

	/* ポーズメニューを開く */
	void PressedActionPoseMenu();

	/* ジャンプ開始 */
	void StartJump();

	/* ジャンプ停止 */
	void StopJump();

	/* 前進/後進 */
	void MoveForward(const FInputActionValue& Value);

	/* 右移動/左移動 */
	void MoveRight(const FInputActionValue& Value);

	/* 視点移動 */
	void Look(const FInputActionValue& Value);

protected:
	/** 弾丸を発射する */
	UFUNCTION(BlueprintCallable)
	void OnFire();

	UFUNCTION()
	void HandleAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	/** タイムライン の スタート */
	UFUNCTION(BlueprintCallable, Category = Timeline)
	void StartTimeline();

	/** タイムライン の リバース */
	UFUNCTION(BlueprintCallable, Category = Timeline)
	void ReverseTimeline();

	/** タイムラインの進捗が変化した際の処理 */
	UFUNCTION()
	void TimelineStep(float value);

	/** 発砲時のアニメーションを出す関数 */
	UFUNCTION(BlueprintCallable, Category = ThirdPerson)
	void AnimationAtFiring();

	/*
	 * 武器を外す関数
	 * @param weapon 武器メッシュの配列
	 * @param number 現在装備している武器の index
	 * @return 外す武器のメッシュを返す
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = ThirdPerson)
	USkeletalMeshComponent* RemoveWeapon(TArray<USkeletalMeshComponent*> Arms, int Number);

	/*
	 * 装備した武器を数える関数
	 * @param weapon 武器メッシュの配列
	 * @param number 現在装備している武器の index
	 */
	UFUNCTION(BlueprintCallable, Category = ThirdPerson)
	void CountWeapon(TArray<USkeletalMeshComponent*> Arms, int Number);

	/*
	 * 武器を装備する関数
	 * @param HasWeapon 武器を装備しているか？
	 * @param HasPistol ピストルを装備しているか？
	 * @param SoketName 武器をアタッチする場所の名前
	 */
	UFUNCTION(BlueprintCallable, Category = ThirdPerson)
	void EquipWeapon(bool bHasWeapon, bool bHasPistol, FName SoketName);

protected:
	/** タイムライン */
	FTimeline* RunTimeline;

	/** カーブ */
	TObjectPtr<class UCurveFloat> RunCurve;

public:
	AGUNMANCharacter();

	virtual void Tick(float DeltaTime) override;

	/** CameraBoom の getter **/
	FORCEINLINE TObjectPtr<class USpringArmComponent> GetCameraBoom() const { return CameraBoom; }
	/** ThirdPersonCamera の getter **/
	FORCEINLINE TObjectPtr<class UCameraComponent> GetThirdPersonCamera() const { return ThirdPersonCamera; }

	/** Mesh1P の getter **/
	TObjectPtr<USkeletalMeshComponent> GetMesh1P() const { return Mesh1P; }

	/** FirstPersonCamera の getter **/
	TObjectPtr<UCameraComponent> GetFirstPersonCamera() const { return FirstPersonCamera; }

	int GetKillCount() const { return KillCount; }

	float GetMaxHealth() const { return MaxHealth; }

	float GetCurrentHealth() const { return CurrentHealth; }

	float GetHealthPercent() const { return CurrentHealth / MaxHealth; }

	/** MaxWalkSpeed の Setter */
	void SetMaxWalkSpeed(float NewSpeed);

	void SetTargetArmLength(float Length);

	void SetOrientRotationToMovement(bool bOrientRotationToMovement);

	void SetUseControllerRotationYaw(bool bYawRotation);

public:
	/* MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;
};
