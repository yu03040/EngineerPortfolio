// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ArmedWeapon/WeaponStructure.h"
#include "Components/TimelineComponent.h"
#include "UMG/UICharacter.h"
#include "GameFramework/Character.h"
#include "GUNMANCharacter.generated.h"


UCLASS(config = Game)
class AGUNMANCharacter : public ACharacter
{
	GENERATED_BODY()

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

	/* ThirdPerson の武器 */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = WeaponSettings, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Weapon;

	/* 玉が着弾したときのエフェクト */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UParticleSystem> WeaponEmitter;

	/* ThirdPerson の武器の配列 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = WeaponSettings, meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<USkeletalMeshComponent>> WeaponMeshes;

	/* ThirdPerson の武器の index */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponSettings, meta = (AllowPrivateAccess = "true"))
	int WeaponNumber;

	/* FirstPerson の武器の攻撃力 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponSettings, meta = (AllowPrivateAccess = "true"))
	float FP_WeaponATK = 5.0f;

	/* ThirdPerson の武器の攻撃力 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponSettings, meta = (AllowPrivateAccess = "true"))
	float TP_WeaponATK;

	/* ThirdPerson のライフルの攻撃力 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponSettings, meta = (AllowPrivateAccess = "true"))
	float TP_RifleATK = 5.0f;

	/* ThirdPerson のショットガンの攻撃力 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponSettings, meta = (AllowPrivateAccess = "true"))
	float TP_ShotgunATK = 15.0f;

	/* ThirdPerson のピストルの攻撃力 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponSettings, meta = (AllowPrivateAccess = "true"))
	float TP_PistolATK = 10.0f;

	/* ThirdPerson の武器のカウント */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponSettings, meta = (AllowPrivateAccess = "true"))
	int WeaponNumberCounter;

	/* 武器を持っているか */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponSettings, meta = (AllowPrivateAccess = "true"))
	bool HasWeapon;

	/* 装備している武器のメッシュ */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponSettings, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> EquippedWeapon;

	/* ThirdPerson のアニメーションインスタンスをセットする */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponSettings, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimInstance> TPMeshAnimInstance;

	/* 装備した武器の情報 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = WeaponSettings, meta = (AllowPrivateAccess = "true"))
	FWeaponStructure EquippedWeaponInformation;

	/* 武器を構えているか */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponSettings, meta = (AllowPrivateAccess = "true"))
	bool IsAiming;

	/* 銃口とキャラクターの位置のオフセット */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	FVector GunOffset;

	/* 発射サウンドエフェクト */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundBase> FireSound;

	/* 発射アニメーション */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> FireAnimation;

	/* フリップフロップのための変数 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	bool bIsFlipped = true;

	/* FirstPerson かどうか */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	bool isFP = false;

	/* ジャンプボタンが押されたかどうか */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	bool JumpButtonDown = false;

	/* プレイヤーの体力 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	float MaxHealth = 1000.0f;

	/* 現在のプレイヤ―の体力 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	float CurrentHealth = MaxHealth;

	/* 死んだプレイヤ―の体力 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	float DeadHealth = 0.0f;

	/* 倒した敵の数 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	int KillCount = 0;

	/* FirstPerson の武器がスポーンする銃弾 */
	UPROPERTY(EditDefaultsOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AfirstpersonProjectProjectile> ProjectileClass;

	/* BattleMapScript の参照 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class ABattleMapScript> BattleMapRef;

	/* プレイヤ―の速さの最小値 */
	UPROPERTY()
	float StartSpeed = 300.0f;

	/* プレイヤ―の速さの最大値 */
	UPROPERTY()
	float EndSpeed = 600.0f;

	/* UICharacter の参照 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UUICharacter> UIRef;

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

protected:
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* TPS と FPS の視点を切り替える */
	void ToggleBetweenTPSAndFPS();

	/* 武器の切り替えと装備 */
	void SwitchingAndEquippingWeapons();

	/* ポーズメニューを開く */
	void PressedActionPoseMenu();

	/* ジャンプ開始 */
	void StartJump();

	/* ジャンプ停止 */
	void StopJump();

	/* 前進/後進 */
	void MoveForward(float Value);

	/* 右移動/左移動 */
	void MoveRight(float Value);

	/**
	 * 左右の視点移動
	 * @param Rate	正規化された割合、すなわち、 1.0 は希望する回転率の 100％
	 */
	void TurnAtRate(float Rate);

	/**
	 * 上下の視点移動
	 * @param Rate	正規化された割合、すなわち、 1.0 は希望する回転率の 100％
	 */
	void LookUpAtRate(float Rate);

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
	UFUNCTION(BlueprintCallable, Category = WeaponSettings)
	void AnimationAtFiring();

protected:
	/** タイムライン */
	FTimeline* RunTimeline;

	/** カーブ */
	TObjectPtr<class UCurveFloat> RunCurve;
};
