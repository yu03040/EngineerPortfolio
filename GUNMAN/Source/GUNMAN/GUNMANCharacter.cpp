// Copyright Epic Games, Inc. All Rights Reserved.

#include "GUNMANCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/SceneComponent.h"
#include "Components/TimelineComponent.h"
#include "Math/Vector.h"
#include "Engine/DataTable.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerInput.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "DrawDebugHelpers.h"
#include "firstpersonProjectProjectile.h"
#include "ArmedWeapon/Rifle.h"
#include "Enemy/AIEnemy.h"
#include "UMG/UICharacter.h"
#include "LevelScript/BattleMapScript.h"


//////////////////////////////////////////////////////////////////////////
// AGUNMANCharacter

AGUNMANCharacter::AGUNMANCharacter()
{
	// フレームごとに Tick() を呼び出すように、このアクタを設定する
	// 必要なければ、これをオフにしてパフォーマンスを向上させることもできる。
	PrimaryActorTick.bCanEverTick = true;

	// collision capsule のサイズを設定する
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// turn rates を設定する
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// コントローラーが回転しても回転させない
	// カメラに影響を与えるだけにする
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// キャラクターの動きを設定する
	GetCharacterMovement()->bOrientRotationToMovement = true; // 入力された方向にキャラクターが動く...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...この回転率で
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// カメラブームの作成（衝突した場合、プレーヤーに引き寄せられる）
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// 三人称カメラコンポーネントの作成
	ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));
	ThirdPersonCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	ThirdPersonCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// 一人称カメラコンポーネントの作成
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(GetCapsuleComponent());
	FirstPersonCamera->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCamera->bUsePawnControlRotation = true;

	// 1人称視点から見たときに使用されるメッシュコンポーネントを作成する。(このポーンをコントロールするとき)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCamera);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	// 一人称視点の銃メッシュコンポーネントを作成する
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(false);
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	FP_Gun->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// 投射物がスポーンするキャラクタの位置からのデフォルトオフセット
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	// 三人称視点の武器メッシュコンポーネントを作成
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh());

	// タイムライン初期化
	RunTimeline = new FTimeline();

	// タイムラインモード設定
	RunTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_TimelineLength);
	RunTimeline->SetTimelineLength(1.0f);

	// カーブアセットの取得
	const ConstructorHelpers::FObjectFinder<UCurveFloat> Find(TEXT("CurveFloat'/Game/Blueprint/ThirdPersonCPP/Blueprints/CB_Run.CB_Run'"));
	if (Find.Succeeded())
	{
		RunCurve = Find.Object;
	}

	// タイムライン更新時のデリゲート
	FOnTimelineFloat TimelineProgress;
	TimelineProgress.BindUFunction(this, TEXT("TimelineStep"));
	RunTimeline->AddInterpFloat(RunCurve, TimelineProgress);
}

void AGUNMANCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// タイムライン実行処理
	if (RunTimeline != nullptr && RunTimeline->IsPlaying())
	{
		RunTimeline->TickTimeline(DeltaTime);
	}
}

void AGUNMANCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// プレイヤーコントローラーを取得
	TObjectPtr<APlayerController> PlayerControllerRef = UGameplayStatics::GetPlayerController(this, 0);
	// マウスを非表示
	PlayerControllerRef->bShowMouseCursor = false;
	PlayerControllerRef->SetInputMode(FInputModeGameOnly());

	// ThirdPerson のアニメーションインスタンスをセット
	TPMeshAnimInstance = GetMesh()->GetAnimInstance();

	// 最初はThirdPerson のカメラをオン
	ThirdPersonCamera->SetActive(true, false);
	FirstPersonCamera->SetActive(false, false);

	bUseControllerRotationYaw = false;

	// Character クラスの Mesh(ThirdPersonのメッシュ) を取得
	TObjectPtr<USkeletalMeshComponent> MeshComponent = GetMesh();

	// ThirdPerson のメッシュを表示
	MeshComponent->SetVisibility(true);

	// FirstPerson のメッシュと銃を非表示
	Mesh1P->SetVisibility(false);
	FP_Gun->SetVisibility(false);

	// AnyDamageデリゲートをバインド
	OnTakeAnyDamage.AddDynamic(this, &AGUNMANCharacter::HandleAnyDamage);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AGUNMANCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	// ジャンプのバインド
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AGUNMANCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AGUNMANCharacter::StopJump);

	// 走る操作へのバインド
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AGUNMANCharacter::StartTimeline);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &AGUNMANCharacter::ReverseTimeline);

	// 攻撃のバインド
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AGUNMANCharacter::OnFire);

	// TPS/FPS 視点切り替えのバインド
	PlayerInputComponent->BindAction("Toggle", IE_Pressed, this, &AGUNMANCharacter::ToggleBetweenTPSAndFPS);

	// 武器装備切り替えのバインド
	PlayerInputComponent->BindAction("SwitchAndEquipWeapons", IE_Pressed, this, &AGUNMANCharacter::SwitchingAndEquippingWeapons);

	// ポーズメニューのバインド
	PlayerInputComponent->BindAction("PauseMenu", IE_Pressed, this, &AGUNMANCharacter::PressedActionPoseMenu);

	// 移動操作のバインド
	PlayerInputComponent->BindAxis("MoveForward", this, &AGUNMANCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGUNMANCharacter::MoveRight);

	// 回転バインディングには2つのバージョンがあり、異なる種類のデバイスに対応できるようになっている
	// turn はマウスのような絶対差分を提供するデバイスを扱う
	// turnrate はアナログジョイスティックのような、変化率として扱うことを選択したデバイスのためのもの
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AGUNMANCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AGUNMANCharacter::LookUpAtRate);
}

void AGUNMANCharacter::TimelineStep(float value)
{
	float NewSpeed = FMath::Lerp(StartSpeed, EndSpeed, value);
	SetMaxWalkSpeed(value);
}

void AGUNMANCharacter::OnFire()
{
	// FirstPerson だったら
	if (isFP)
	{
		// 腕のメッシュのアニメーションオブジェクトを取得
		TObjectPtr<UAnimInstance> AnimInstance = Mesh1P->GetAnimInstance();

		if (AnimInstance != nullptr)
		{
			// 発射するときのアニメーション
			AnimInstance->Montage_Play(FireAnimation);
		}

		// 指定された場合、サウンドを再生
		if (FireSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
		}

		// 玉を発射
		if (ProjectileClass != nullptr)
		{
			TObjectPtr<UWorld> const World = GetWorld();
			if (World != nullptr)
			{
				const FRotator SpawnRotation = GetControlRotation();

				// MuzzleOffset はカメラ空間にあるため、
				// キャラクター位置からオフセットする前にワールド空間に変換し、最終的な銃口位置を求める。
				const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

				//スポーン衝突処理のオーバーライドを設定する
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

				// ノズルから玉を発射する
				World->SpawnActor<AfirstpersonProjectProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

			}
		}

		// ヒット結果を格納する構造体
		FHitResult HitResult;

		// ライントレースの開始位置
		FVector StartLocation = ThirdPersonCamera->GetComponentLocation();

		// ライントレースの終了位置（ここでは、開始位置から前方に 10000cm 進んだ位置を指定）
		FVector EndLocation = StartLocation + ThirdPersonCamera->GetForwardVector() * 10000.0f;

		// 衝突クエリに関するパラメータを設定するための構造体
		FCollisionQueryParams TraceParams(FName(TEXT("LineTraceByChannel")), true, this);

		TraceParams.bTraceComplex = false; // 詳細な衝突判定を行うか
		TraceParams.AddIgnoredActor(this); // このアクターを無視する

		// コリジョンチャンネル：オブジェクトが衝突やトレースに対してどのように反応するかを設定するためのカテゴリ
		// ECC_Visibility：視覚的なトレースやレイキャストに使用されるチャンネル

		// ライントレースを実行
		bool bHit = GetWorld()->LineTraceSingleByChannel(
			HitResult,
			StartLocation,
			EndLocation,
			ECC_Visibility,
			TraceParams
		);

		if (bHit)
		{
			// ヒットした場所の情報を使用
			FVector HitLocation = HitResult.Location;
			UGameplayStatics::SpawnEmitterAtLocation(
				this,					// エミッターを生成するアクター（GUNMANCharacter自身）
				WeaponEmitter,			// エミッターテンプレート
				HitLocation,			// エミッターの生成位置
				FRotator::ZeroRotator,	// エミッターの初期回転
				true,					// 再生完了後、自動でエミッターを破棄する
				EPSCPoolMethod::None,	// エミッターを再利用しない
				true					// スポーン時にアクティブ
			);

			// ヒットした敵の情報を使用
			TObjectPtr<AActor> HitEnemy = HitResult.GetActor();
			// アクターが敵だったら
			TObjectPtr<AAIEnemy> Enemy = Cast<AAIEnemy>(HitEnemy);
			if (Enemy)
			{
				TObjectPtr<AController> EventInstigator = NULL;
				TObjectPtr<AActor> DamageCauser = NULL;
				TSubclassOf<UDamageType> DamageTypeClass = NULL;
				if (Enemy->GetCurrentHealth() > 0.0f)
				{
					UGameplayStatics::ApplyDamage(Enemy, FP_WeaponATK, EventInstigator, DamageCauser, DamageTypeClass);
					if (Enemy->GetCurrentHealth() <= 0.0f)
					{
						KillCount++;
					}
				}
			}
		}
	}
	else
	{
		// ThirdPersonだったら

		// ヒット結果を格納する構造体
		FHitResult HitResult;

		// ライントレースの開始位置
		FVector StartLocation = ThirdPersonCamera->GetComponentLocation();

		// ライントレースの終了位置（ここでは、開始位置から前方に 10000cm 進んだ位置を指定）
		FVector EndLocation = StartLocation + ThirdPersonCamera->GetForwardVector() * 10000.0f;

		// 衝突クエリに関するパラメータを設定するための構造体
		FCollisionQueryParams TraceParams(FName(TEXT("LineTraceByChannel")), true, this);

		TraceParams.bTraceComplex = false; // 詳細な衝突判定を行うか
		TraceParams.AddIgnoredActor(this); // このアクターを無視する

		// コリジョンチャンネル：オブジェクトが衝突やトレースに対してどのように反応するかを設定するためのカテゴリ
		// ECC_Visibility：視覚的なトレースやレイキャストに使用されるチャンネル

		// ライントレースを実行
		bool bHit = GetWorld()->LineTraceSingleByChannel(
			HitResult,
			StartLocation,
			EndLocation,
			ECC_Visibility,
			TraceParams
		);

		// サウンドを再生
		if (FireSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
		}

		if (bHit)
		{
			// ヒットした場所の情報を使用
			FVector HitLocation = HitResult.Location;
			UGameplayStatics::SpawnEmitterAtLocation(
				this,					// エミッターを生成するアクター（GUNMANCharacter自身）
				WeaponEmitter,				// エミッターテンプレート
				HitLocation,			// エミッターの生成位置
				FRotator::ZeroRotator,	// エミッターの初期回転
				true,					// 再生完了後、自動でエミッターを破棄する
				EPSCPoolMethod::None,	// エミッターを再利用しない
				true					// スポーン時にアクティブ
			);

			// ヒットした敵の情報を使用
			TObjectPtr<AActor> HitEnemy = HitResult.GetActor();
			// アクターが敵だったら
			TObjectPtr<AAIEnemy> Enemy = Cast<AAIEnemy>(HitEnemy);
			if (Enemy)
			{
				TObjectPtr<AController> EventInstigator = NULL;
				AActor* DamageCauser = NULL;
				TSubclassOf<UDamageType> DamageTypeClass = NULL;
				if (Enemy->GetCurrentHealth() > 0.0f)
				{
					if (WeaponNumber == 0) //ピストル
					{
						TP_WeaponATK = TP_PistolATK;
					}
					else if (WeaponNumber == 1) // ライフル
					{
						TP_WeaponATK = TP_RifleATK;
					}
					else // ショットガン
					{
						TP_WeaponATK = TP_ShotgunATK;
					}
					// 10 ダメージ(WeaponATK分)を与える
					UGameplayStatics::ApplyDamage(Enemy, TP_WeaponATK, EventInstigator, DamageCauser, DamageTypeClass);
					if (Enemy->GetCurrentHealth() <= 0.0f)
					{
						KillCount++;
					}
				}
			}
		}

	}
}

void AGUNMANCharacter::ToggleBetweenTPSAndFPS()
{
	// フリップフロップ作成
	if (bIsFlipped)
	{
		// FirstPerson
		// FirstPerson のカメラをオン
		ThirdPersonCamera->SetActive(false);
		FirstPersonCamera->SetActive(true);

		bUseControllerRotationYaw = true;

		// Character クラスの Mesh(ThirdPersonのメッシュ) を取得
		USkeletalMeshComponent* MeshComponent = GetMesh();

		// ThirdPerson のメッシュを非表示
		MeshComponent->SetVisibility(false);
		Weapon->SetVisibility(false);
		TArray<TObjectPtr<AActor>> WeaponList; // レベルから武器を探して取得
		UGameplayStatics::GetAllActorsOfClass(this, ARifle::StaticClass(), WeaponList);
		for (TObjectPtr<AActor> SearchActor : WeaponList)
		{
			TObjectPtr<ARifle> NewActor = Cast<ARifle>(SearchActor);
			NewActor->SetActorHiddenInGame(true);
		}

		// FirstPerson のメッシュと銃を表示
		Mesh1P->SetVisibility(true);
		FP_Gun->SetVisibility(true);

		// FirstPerson だったら
		isFP = true;

		bIsFlipped = false;
	}
	else
	{
		// ThirdPerson
		// ThirdPerson のカメラをオン
		ThirdPersonCamera->SetActive(true);
		FirstPersonCamera->SetActive(false);

		bUseControllerRotationYaw = false;

		// Character クラスの Mesh(ThirdPersonのメッシュ) を取得
		TObjectPtr<USkeletalMeshComponent> MeshComponent = GetMesh();

		// ThirdPerson のメッシュを表示
		MeshComponent->SetVisibility(true);
		Weapon->SetVisibility(false);
		TArray<TObjectPtr<AActor>> WeaponList; // レベルから武器を探して取得
		UGameplayStatics::GetAllActorsOfClass(this, ARifle::StaticClass(), WeaponList);
		for (TObjectPtr<AActor> SearchActor : WeaponList)
		{
			TObjectPtr<ARifle> NewActor = Cast<ARifle>(SearchActor);
			NewActor->SetActorHiddenInGame(false);
		}

		// FirstPerson のメッシュと銃を非表示
		Mesh1P->SetVisibility(false);
		FP_Gun->SetVisibility(false);

		// ThirdPerson だったら
		isFP = false;

		bIsFlipped = true;
	}
}

void AGUNMANCharacter::SwitchingAndEquippingWeapons()
{
}

void AGUNMANCharacter::PressedActionPoseMenu()
{
	// ワールド情報があるか
	TObjectPtr<UWorld> World = GetWorld();
	if (World)
	{
		// レベルがあるか確認後、ポーズメニュー開いたときの最初のボタンの位置を初期化
		BattleMapRef = Cast<ABattleMapScript>(World->GetLevelScriptActor());
		if (BattleMapRef)
		{
			BattleMapRef->InitializeButtonPosition();
		}
	}
}

void AGUNMANCharacter::StartJump()
{
	// ジャンプする
	Jump();

	// ジャンプしたか
	JumpButtonDown = CanJump();

	// MovementComponent が CharacterMovementComponent だったら
	TObjectPtr<UCharacterMovementComponent> MovementComponent = Cast<UCharacterMovementComponent>(GetMovementComponent());
	if (MovementComponent)
	{
		// ベクトルの長さが0以上か
		if (MovementComponent->Velocity.Size() > 0.0f)
		{
			MovementComponent->JumpZVelocity = 400.0f;
		}
		else
		{
			MovementComponent->JumpZVelocity = 400.0f;
		}

	}
}

void AGUNMANCharacter::StopJump()
{
	// ジャンプをやめる
	StopJumping();

	// ジャンプしていないのでオフ
	JumpButtonDown = false;

	// MovementComponent が CharacterMovementComponent だったら
	UCharacterMovementComponent* MovementComponent = Cast<UCharacterMovementComponent>(GetMovementComponent());
	if (MovementComponent)
	{
		// ベクトルの長さが0以上か
		if (MovementComponent->Velocity.Size() > 0.0f)
		{
			MovementComponent->JumpZVelocity = 400.0f;
		}
		else
		{
			MovementComponent->JumpZVelocity = 400.0f;
		}

	}
}

void AGUNMANCharacter::HandleAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	// ダメージ分、体力を減らす
	CurrentHealth -= Damage;
	// 体力がなくなったら
	if (CurrentHealth <= DeadHealth)
	{
		UGameplayStatics::OpenLevel(this, "GameOverMap");
	}
}

void AGUNMANCharacter::StartTimeline()
{
	// タイムライン開始
	if (RunTimeline != nullptr)
	{
		RunTimeline->PlayFromStart();
	}
}

void AGUNMANCharacter::ReverseTimeline()
{
	// タイムラインをリバース
	if (RunTimeline != nullptr)
	{
		RunTimeline->ReverseFromEnd();
	}
}

void AGUNMANCharacter::SetMaxWalkSpeed(float NewSpeed)
{ 
	GetCharacterMovement()->MaxWalkSpeed = NewSpeed; 
}

void AGUNMANCharacter::AnimationAtFiring()
{
	// 発砲音を出す
	UGameplayStatics::PlaySound2D(this, EquippedWeaponInformation.GunshotSound);
	// 発砲炎アニメーションを出す
	UGameplayStatics::SpawnEmitterAttached(EquippedWeaponInformation.MuzzleFire, EquippedWeapon, EquippedWeaponInformation.MuzzleFireSoketName);
	// 身体の動くアニメーションを出す
	PlayAnimMontage(EquippedWeaponInformation.FiringMontage, 1.0f);
	GetWorld()->SpawnActor<ARifleAmmunition>(
		EquippedWeaponInformation.AmmunitionClass,
		FTransform(EquippedWeapon->GetSocketTransform(TEXT("None")))
	);
}

void AGUNMANCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AGUNMANCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AGUNMANCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// どちらが前方向か調べる
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// 前方向ベクトルの取得
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		// その方向に移動する
		AddMovementInput(Direction, Value);
	}
}

void AGUNMANCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// どちらが右方向か調べる
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// 右方向ベクトルの取得 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// その方向に移動する
		AddMovementInput(Direction, Value);
	}
}