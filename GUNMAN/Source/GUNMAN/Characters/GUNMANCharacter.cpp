// Copyright Epic Games, Inc. All Rights Reserved.

#include "GUNMANCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/SceneComponent.h"
#include "Components/TimelineComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "Math/Vector.h"
#include "Engine/DataTable.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerInput.h"
#include "UObject/ConstructorHelpers.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "GUNMAN/SpawnActors/firstpersonProjectProjectile.h"
#include "GUNMAN/ArmedWeapon/Weapon.h"
#include "GUNMAN/Enemy/AIEnemy.h"
#include "GUNMAN/LevelScript/BattleMapScript.h"


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

	// StimuliSourceComponent を作成
	StimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("StimuliSourceComponent"));
	if (StimuliSourceComponent)
	{
		// 敵AI がプレイヤーを完治できるように視覚に登録する
		StimuliSourceComponent->RegisterForSense(TSubclassOf<UAISense_Sight>());
		StimuliSourceComponent->RegisterWithPerceptionSystem();
	}

	// Enhanced Input のアセットをロード
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> MappingContextFinder(TEXT("/Game/GUNMAN/Input/IMC_Default.IMC_Default"));
	if (MappingContextFinder.Succeeded())
	{
		DefaultMappingContext = MappingContextFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> JumpActionFinder(TEXT("/Game/GUNMAN/Input/IA_Jump.IA_Jump"));
	if (JumpActionFinder.Succeeded())
	{
		JumpAction = JumpActionFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> FireActionFinder(TEXT("/Game/GUNMAN/Input/IA_Fire.IA_Fire"));
	if (FireActionFinder.Succeeded())
	{
		FireAction = FireActionFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> ToggleActionFinder(TEXT("/Game/GUNMAN/Input/IA_Toggle.IA_Toggle"));
	if (ToggleActionFinder.Succeeded())
	{
		ToggleAction = ToggleActionFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> ReadyGunActionFinder(TEXT("/Game/GUNMAN/Input/IA_ReadyGun.IA_ReadyGun"));
	if (ReadyGunActionFinder.Succeeded())
	{
		ReadyGunAction = ReadyGunActionFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> AttachGunActionFinder(TEXT("/Game/GUNMAN/Input/IA_SwitchAndEquipWeapons.IA_SwitchAndEquipWeapons"));
	if (AttachGunActionFinder.Succeeded())
	{
		AttachGunAction = AttachGunActionFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> RunActionFinder(TEXT("/Game/GUNMAN/Input/IA_Run.IA_Run"));
	if (RunActionFinder.Succeeded())
	{
		RunAction = RunActionFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> PauseMenuActionFinder(TEXT("/Game/GUNMAN/Input/IA_PauseMenu.IA_PauseMenu"));
	if (PauseMenuActionFinder.Succeeded())
	{
		PauseMenuAction = PauseMenuActionFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> MoveForwardActionFinder(TEXT("/Game/GUNMAN/Input/IA_MoveForward.IA_MoveForward"));
	if (MoveForwardActionFinder.Succeeded())
	{
		MoveForwardAction = MoveForwardActionFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> MoveRightActionFinder(TEXT("/Game/GUNMAN/Input/IA_MoveRight.IA_MoveRight"));
	if (MoveRightActionFinder.Succeeded())
	{
		MoveRightAction = MoveRightActionFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> LookActionFinder(TEXT("/Game/GUNMAN/Input/IA_Look.IA_Look"));
	if (LookActionFinder.Succeeded())
	{
		LookAction = LookActionFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableFinder(TEXT("/Game/GUNMAN/Blueprint/ArmedWeapon/DT_Weapon.DT_Weapon"));
	if (DataTableFinder.Succeeded())
	{
		WeaponDataTable = DataTableFinder.Object;
	}

	// タイムライン初期化
	RunTimeline = new FTimeline();

	// タイムラインモード設定
	RunTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_TimelineLength);
	RunTimeline->SetTimelineLength(1.0f);

	// カーブアセットの取得
	const ConstructorHelpers::FObjectFinder<UCurveFloat> Find(TEXT("CurveFloat'/Game/GUNMAN/Blueprint/Characters/CB_Run.CB_Run'"));
	if (Find.Succeeded())
	{
		RunCurve = Find.Object;
	}

	// タイムライン更新時のデリゲート
	FOnTimelineFloat TimelineProgress;
	TimelineProgress.BindUFunction(this, TEXT("TimelineStep"));
	RunTimeline->AddInterpFloat(RunCurve, TimelineProgress);
}

void AGUNMANCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// プレイヤーコントローラーを取得
	TObjectPtr<APlayerController> PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	// Enhanced Input サブシステムにマッピングコンテキストを追加
	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// マウスを非表示
	PlayerController->bShowMouseCursor = false;
	PlayerController->SetInputMode(FInputModeGameOnly());

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

	// WBP のパスをセット
	CharacterWidgetPath = "/Game/GUNMAN/Blueprint/UMG/WBP_UICharacter.WBP_UICharacter_C";

	// パスからウィジェットを生成する
	CharacterWidgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*CharacterWidgetPath)).LoadSynchronous();

	// ウィジェットを表示する
	if (IsValid(CharacterWidgetClass))
	{
		UICharacterRef = Cast<UUICharacter>(CreateWidget(PlayerController, CharacterWidgetClass));

		if (UICharacterRef)
		{
			UICharacterRef->AddToViewport();
		}
	}

	// WBP のパスをセット
	GunSightWidgetPath = "/Game/GUNMAN/Blueprint/UMG/WBP_UIGunSight.WBP_UIGunSight_C";

	// パスからウィジェットを生成する
	GunSightWidgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*GunSightWidgetPath)).LoadSynchronous();

	// ウィジェットを表示する
	if (IsValid(GunSightWidgetClass))
	{
		UIGunSightRef = Cast<UUIGunSight>(CreateWidget(PlayerController, GunSightWidgetClass));
	}
}

void AGUNMANCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// 銃を腕にアタッチする
	FP_Gun->AttachToComponent(
		Mesh1P, 
		FAttachmentTransformRules(
			EAttachmentRule::SnapToTarget, 
			EAttachmentRule::SnapToTarget, 
			EAttachmentRule::SnapToTarget, 
			false
		), 
		FName(TEXT("GripPoint"))
	);
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

//////////////////////////////////////////////////////////////////////////
// Input

void AGUNMANCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	if (TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// ジャンプのバインド
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AGUNMANCharacter::StartJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AGUNMANCharacter::StopJump);

		// 攻撃のバインド
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &AGUNMANCharacter::StartFire);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &AGUNMANCharacter::StopFire);

		// TPS/FPS 視点切り替えのバインド
		EnhancedInputComponent->BindAction(ToggleAction, ETriggerEvent::Triggered, this, &AGUNMANCharacter::ToggleBetweenTPSAndFPS);

		// 武器を構えるバインド
		EnhancedInputComponent->BindAction(ReadyGunAction, ETriggerEvent::Triggered, this, &AGUNMANCharacter::StartReadyGun);
		EnhancedInputComponent->BindAction(ReadyGunAction, ETriggerEvent::Completed, this, &AGUNMANCharacter::StopReadyGun);

		// 武器のつけ外しのバインド
		EnhancedInputComponent->BindAction(AttachGunAction, ETriggerEvent::Triggered, this, &AGUNMANCharacter::AttachingAndRemovingGun);

		// 走る操作へのバインド
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &AGUNMANCharacter::StartTimeline);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &AGUNMANCharacter::ReverseTimeline);

		//// ポーズメニューのバインド
		EnhancedInputComponent->BindAction(PauseMenuAction, ETriggerEvent::Triggered, this, &AGUNMANCharacter::PressedActionPoseMenu);

		// 移動操作のバインド
		EnhancedInputComponent->BindAction(MoveForwardAction, ETriggerEvent::Triggered, this, &AGUNMANCharacter::MoveForward);
		EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &AGUNMANCharacter::MoveRight);

		// 視点移動のバインド
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGUNMANCharacter::Look);
	}
}

void AGUNMANCharacter::StartFire()
{
	FTimerManager& FireTimerManager = GetWorldTimerManager();
	FireTimerManager.SetTimer
	(
		FireTimerHandle,
		this,
		&AGUNMANCharacter::FiringEvent,
		FiringInterval,
		true
	);
}

void AGUNMANCharacter::FiringEvent()
{
	if ((HasWeapon && IsAiming && CanAttack) || isFP)
	{
		if (isFP == false)
		{
			// 発砲炎アニメーション再生(Third Person)
			AnimationAtFiring();
			OnFire();
		}
		else
		{
			OnFire();
		}
	}
}

void AGUNMANCharacter::StopFire()
{
	FTimerManager& FireTimerManager = GetWorldTimerManager();

	// タイマーがアクティブであるかチェックしてからキャンセル
	if (FireTimerManager.IsTimerActive(FireTimerHandle))
	{
		// タイマーをクリア
		FireTimerManager.ClearTimer(FireTimerHandle);

		// タイマーハンドルを無効化
		FireTimerHandle.Invalidate();
	}
}

void AGUNMANCharacter::FireState_Implementation(bool CanATK)
{
	CanAttack = CanATK;
}

void AGUNMANCharacter::AttachWeapon_Implementation(USkeletalMeshComponent* WeaponMesh, FName AttachSoketName)
{
	// 武器をアタッチ
	WeaponMeshes.Add(WeaponMesh);
	WeaponMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), AttachSoketName);
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
		UGameplayStatics::GetAllActorsOfClass(this, AWeapon::StaticClass(), WeaponList);
		for (TObjectPtr<AActor> SearchActor : WeaponList)
		{
			TObjectPtr<AWeapon> NewActor = Cast<AWeapon>(SearchActor);
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
		UGameplayStatics::GetAllActorsOfClass(this, AWeapon::StaticClass(), WeaponList);
		for (TObjectPtr<AActor> SearchActor : WeaponList)
		{
			TObjectPtr<AWeapon> NewActor = Cast<AWeapon>(SearchActor);
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

void AGUNMANCharacter::StartReadyGun()
{
	if (HasWeapon == true)
	{
		GunPreparationProcess(true, 150.0f, false, true, FVector(0.0f, 98.0f, 102.0f), FRotator(0.0f, 32.0f, 41.0f));
		if (UIGunSightRef)
		{
			UIGunSightRef->AddToViewport();
		}
	}
}

void AGUNMANCharacter::GunPreparationProcess(bool bIsAiming, float ArmLength, bool bOrientRotationToMovement, bool bYawRotation, FVector CameraBoomLocation, FRotator CameraBoomRotation)
{
	// エイムのフラグを設定
	IsAiming = bIsAiming;

	// エイムの状態をアニメーション（ThirdPerson）に反映
	IAnimationInterface* AnimInterface = Cast<IAnimationInterface>(TPMeshAnimInstance);
	AnimInterface->AimingState_Implementation(IsAiming);

	// カメラアームの長さを設定
	SetTargetArmLength(ArmLength);

	// カメラの向きの設定
	SetOrientRotationToMovement(bOrientRotationToMovement);

	// 歩く向きの設定
	SetUseControllerRotationYaw(bYawRotation);

	// カメラブームの位置と角度を設定
	CameraBoom->SetRelativeLocationAndRotation(CameraBoomLocation, CameraBoomRotation, false, false);
}

void AGUNMANCharacter::StopReadyGun()
{
	GunPreparationProcess(false, 300.0f, true, false, FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f));
	if (UIGunSightRef)
	{
		UIGunSightRef->RemoveFromParent();
	}
}

void AGUNMANCharacter::AttachingAndRemovingGun()
{
	// 武器のインデックスをセット
	WeaponNumber = WeaponNumberCounter;

	// 武器を持っているか？
	if (!HasWeapon)
	{
		// インデックスにあたる武器をセット
		EquippedWeapon = WeaponMeshes[WeaponNumber];

		// 武器を表示
		EquippedWeapon->SetHiddenInGame(false, false);

		// データテーブルにアクセス
		FName RowName = EquippedWeapon->ComponentTags[0];
		FWeaponStructure* Row = WeaponDataTable->FindRow<FWeaponStructure>(RowName, "");
		if (Row)
		{
			// 発砲時の情報をセット
			EquippedWeaponInformation.GunshotSound = Row->GunshotSound;
			EquippedWeaponInformation.MuzzleFire = Row->MuzzleFire;
			EquippedWeaponInformation.MuzzleFireSoketName = Row->MuzzleFireSoketName;
			EquippedWeaponInformation.FiringMontage = Row->FiringMontage;
			EquippedWeaponInformation.AmmunitionClass = Row->AmmunitionClass;
			EquippedWeaponInformation.AmmunitionSocketName = Row->AmmunitionSocketName;

			// 武器を装備してサウンドを再生する
			EquipWeapon(true, Row->HasPistol, Row->EquipSocketName);
			UGameplayStatics::PlaySound2D(this, Row->EquipmentNoise);
		}

		// カウント処理
		CountWeapon(WeaponMeshes, WeaponNumber);
	}
	else
	{
		// 外す武器を選ぶ
		FName RowName = RemoveWeapon(WeaponMeshes, WeaponNumber)->ComponentTags[0];

		// データテーブルにアクセス
		FWeaponStructure* Row = WeaponDataTable->FindRow<FWeaponStructure>(RowName, "");
		if (Row)
		{
			// 武器を外す
			EquipWeapon(false, false, Row->AttachSocketName);
		}
	}
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
			// プレイヤーコントローラーを取得
			TObjectPtr<APlayerController> PlayerController = UGameplayStatics::GetPlayerController(this, 0);

			if (TObjectPtr<UEnhancedInputLocalPlayerSubsystem> Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->RemoveMappingContext(DefaultMappingContext);
				Subsystem->AddMappingContext(BattleMapRef->PauseMenuMappingContext, 0);
			}

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

void AGUNMANCharacter::SetTargetArmLength(float Length)
{
	CameraBoom->TargetArmLength = Length;
}

void AGUNMANCharacter::SetOrientRotationToMovement(bool bOrientRotationToMovement)
{
	GetCharacterMovement()->bOrientRotationToMovement = bOrientRotationToMovement;
}

void AGUNMANCharacter::SetUseControllerRotationYaw(bool bYawRotation)
{
	bUseControllerRotationYaw = bYawRotation;
}

void AGUNMANCharacter::AnimationAtFiring()
{
	// 発砲音を出す
	UGameplayStatics::PlaySound2D(this, EquippedWeaponInformation.GunshotSound);
	// 発砲炎アニメーションを出す
	UGameplayStatics::SpawnEmitterAttached(EquippedWeaponInformation.MuzzleFire, EquippedWeapon, EquippedWeaponInformation.MuzzleFireSoketName);
	// 身体の動くアニメーションを出す
	PlayAnimMontage(EquippedWeaponInformation.FiringMontage, 1.0f);
	GetWorld()->SpawnActor<AWeaponAmmunition>(
		EquippedWeaponInformation.AmmunitionClass,
		FTransform(EquippedWeapon->GetSocketTransform(TEXT("None")))
	);
}

USkeletalMeshComponent* AGUNMANCharacter::RemoveWeapon(TArray<USkeletalMeshComponent*> Arms, int Number)
{
	int AllWeaponNumber = Arms.Num() - 1;
	int CurrentWeaponNumber = Number - 1;
	bool bPickAllWeaponNumber = false;

	if (CurrentWeaponNumber < 0)
	{
		bPickAllWeaponNumber = true;
	}
	else
	{
		bPickAllWeaponNumber = false;
	}

	int PickIndex = UKismetMathLibrary::SelectInt(AllWeaponNumber, CurrentWeaponNumber, bPickAllWeaponNumber);

	return Arms[PickIndex];
}

void AGUNMANCharacter::CountWeapon(TArray<USkeletalMeshComponent*> Arms, int Number)
{
	int AllWeaponNumber = Arms.Num();
	int NextWeaponNumber = Number + 1;
	bool bPickNextWeaponNumber = false;

	if (NextWeaponNumber < AllWeaponNumber)
	{
		bPickNextWeaponNumber = true;
	}
	else
	{
		bPickNextWeaponNumber = false;
	}

	int FirstWeaponIndex = 0;
	WeaponNumberCounter = UKismetMathLibrary::SelectInt(NextWeaponNumber, FirstWeaponIndex, bPickNextWeaponNumber);
}

void AGUNMANCharacter::EquipWeapon(bool bHasWeapon, bool bHasPistol, FName SoketName)
{
	EquippedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), SoketName);

	HasWeapon = bHasWeapon;

	IAnimationInterface* AnimInterface = Cast<IAnimationInterface>(TPMeshAnimInstance);
	AnimInterface->EquippedState_Implementation(HasWeapon, bHasPistol);
}

void AGUNMANCharacter::MoveForward(const FInputActionValue& Value)
{
	if ((Controller != nullptr) && (Value.Get<float>() != 0.0f))
	{
		// どちらが前方向か調べる
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// 前方向ベクトルの取得
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		// その方向に移動する
		AddMovementInput(Direction, Value.Get<float>());
	}
}

void AGUNMANCharacter::MoveRight(const FInputActionValue& Value)
{
	if ((Controller != nullptr) && (Value.Get<float>() != 0.0f))
	{
		// どちらが右方向か調べる
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// 右方向ベクトルの取得 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// その方向に移動する
		AddMovementInput(Direction, Value.Get<float>());
	}
}

void AGUNMANCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// コントローラにヨーとピッチの入力を追加
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}
