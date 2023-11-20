// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ArmedWeapon/WeaponStructure.h"
#include "Components/TimelineComponent.h"
#include "../GUNMAN/UMG/UICharacter.h"
#include "GameFramework/Character.h"
#include "GUNMANCharacter.generated.h"


UCLASS(config = Game)
class AGUNMANCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* ThirdPersonCamera;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCamera;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float BaseLookUpRate;

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = FirstPerson, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = FirstPerson, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* FP_Gun;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = FirstPerson, meta = (AllowPrivateAccess = "true"))
	USceneComponent* FP_MuzzleLocation;

	/** ThirdPerson の武器 */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = WeaponSettings, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Weapon;

	/** 玉が着弾したときのエフェクト */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* WeaponEmitter;

	/** ThirdPerson の武器の配列 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = WeaponSettings, meta = (AllowPrivateAccess = "true"))
	TArray<USkeletalMeshComponent*> WeaponMeshes;

	/** ThirdPerson の武器の index */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponSettings, meta = (AllowPrivateAccess = "true"))
	int WeaponNumber;

	/** ThirdPerson の武器のカウント */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponSettings, meta = (AllowPrivateAccess = "true"))
	int WeaponNumberCounter;

	/** 武器を持っているか */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponSettings, meta = (AllowPrivateAccess = "true"))
	bool HasWeapon;

	/** 装備している武器のメッシュ */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponSettings, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* EquippedWeapon;

	/** ThirdPerson のアニメーションインスタンスをセットする */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponSettings, meta = (AllowPrivateAccess = "true"))
	UAnimInstance* TPMeshAnimInstance;

	/** 装備した武器の情報 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = WeaponSettings, meta = (AllowPrivateAccess = "true"))
	FWeaponStructure EquippedWeaponInformation;

	/** 武器を構えているか */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WeaponSettings, meta = (AllowPrivateAccess = "true"))
	bool IsAiming;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	FVector GunOffset;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* FireAnimation;

	/** フリップフロップのための変数 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	bool bIsFlipped = true;

	/** FirstPerson かどうか */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	bool isFP = false;

	/** ジャンプボタンが押されたかどうか */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	bool JumpButtonDown = false;

	/** プレイヤーの体力 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	float Health = 1000.0f;

	/** 倒した敵の数 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	int KillCount = 0;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AfirstpersonProjectProjectile> ProjectileClass;

public:
	AGUNMANCharacter();

	virtual void Tick(float DeltaTime) override;

	/** UICharacter の リファレンス */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget, meta = (AllowPrivateAccess = "true"))
	UUICharacter* UICharacterRef;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns ThirdPersonCamera subobject **/
	FORCEINLINE class UCameraComponent* GetThirdPersonCamera() const { return ThirdPersonCamera; }

	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }

	/** Returns FirstPersonCamera subobject **/
	UCameraComponent* GetFirstPersonCamera() const { return FirstPersonCamera; }

protected:
	virtual void BeginPlay() override;

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	/** 弾丸を発射する */
	UFUNCTION(BlueprintCallable)
	void OnFire();

	/** TPS と FPS の視点を切り替える */
	void ToggleBetweenTPSAndFPS();

	/** 武器の切り替えと装備 */
	void SwitchingAndEquippingWeapons();

	/** Start to jump. */
	void StartJump();

	/** Stop to jump. */
	void StopJump();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

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

	/** MaxWalkSpeed の Setter */
	UFUNCTION(BlueprintCallable, Category = Movement)
	void SetMaxWalkSpeed(float NewSpeed);

	/** 発砲時のアニメーションを出す関数 */
	UFUNCTION(BlueprintCallable, Category = WeaponSettings)
	void AnimationAtFiring();

	UPROPERTY()
	float StartSpeed;

	UPROPERTY()
	float EndSpeed;

	/** タイムライン */
	FTimeline* RunTimeline;

	/** カーブ */
	class UCurveFloat* RunCurve;

	/** タイマー */
	float Timer;


};
