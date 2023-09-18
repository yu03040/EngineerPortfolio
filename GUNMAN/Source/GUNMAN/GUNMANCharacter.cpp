// Copyright Epic Games, Inc. All Rights Reserved.

#include "GUNMANCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
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

//////////////////////////////////////////////////////////////////////////
// AGUNMANCharacter

AGUNMANCharacter::AGUNMANCharacter()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));
	ThirdPersonCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	ThirdPersonCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create a CameraComponent	
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(GetCapsuleComponent());
	FirstPersonCamera->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCamera->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCamera);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	// Create a firstperson gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(false);			// otherwise won't be visible in the multiplayer
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_Gun->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	// Create a thirdperson weapon mesh component
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

	// 線形補間する値をセット
	StartSpeed = 300.0f;
	EndSpeed = 600.0f;
}

void AGUNMANCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Timer += DeltaTime;

	// 経過時間をセット
	UICharacterRef->DeltaTimes = DeltaTime;
	// 予め決めた制限時間から経過時間を引く
	UICharacterRef->TimeLimit -= UICharacterRef->DeltaTimes;
	// 0 秒以下ならゲーム終了（ゲームオーバー）
	if (UICharacterRef->TimeLimit <= 0.0f)
	{
		UGameplayStatics::OpenLevel(this, "GameOverMap");
	}

	// タイムライン実行処理
	if (RunTimeline != nullptr && RunTimeline ->IsPlaying())
	{
		RunTimeline->TickTimeline(DeltaTime);
	}
}

void AGUNMANCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// プレイヤーコントローラーを取得
	APlayerController* aPlayerController = UGameplayStatics::GetPlayerController(this, 0);
	// マウスを非表示
	aPlayerController->bShowMouseCursor = false;
	aPlayerController->SetInputMode(FInputModeGameOnly());

	Timer = 0.0f;

	// ThirdPerson のアニメーションインスタンスをセット
	TPMeshAnimInstance = GetMesh()->GetAnimInstance();

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	//FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

	// 最初はThirdPerson のカメラをオン
	ThirdPersonCamera->SetActive(true, false);
	FirstPersonCamera->SetActive(false, false);

	bUseControllerRotationYaw = false;

	// Character クラスの Mesh(ThirdPersonのメッシュ) を取得
	USkeletalMeshComponent* MeshComponent = GetMesh();

	// ThirdPerson のメッシュを表示
	MeshComponent->SetVisibility(true);

	// FirstPerson のメッシュと銃を非表示
	Mesh1P->SetVisibility(false);
	FP_Gun->SetVisibility(false);

	// Show or hide the two versions of the gun based on whether or not we're using motion controllers.
	if (bUsingMotionControllers)
	{
		//VR_Gun->SetHiddenInGame(false, true);
		Mesh1P->SetHiddenInGame(true, true);
	}
	else
	{
		//VR_Gun->SetHiddenInGame(true, true);
		Mesh1P->SetHiddenInGame(false, true);
	}

	// AnyDamageデリゲートをバインド
	OnTakeAnyDamage.AddDynamic(this, &AGUNMANCharacter::HandleAnyDamage);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AGUNMANCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	// Bind jump event
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AGUNMANCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AGUNMANCharacter::StopJump);

	// Bind run event
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AGUNMANCharacter::StartTimeline);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &AGUNMANCharacter::ReverseTimeline);

	// Bind crouch event
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AGUNMANCharacter::StartCrouching);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AGUNMANCharacter::StopCrouching);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AGUNMANCharacter::OnFire);

	// Bind Toggle event
	PlayerInputComponent->BindAction("Toggle", IE_Pressed, this, &AGUNMANCharacter::ToggleBetweenTPSAndFPS);

	// Bind Switch And Equip Weapons event
	PlayerInputComponent->BindAction("SwitchAndEquipWeapons", IE_Pressed, this, &AGUNMANCharacter::SwitchingAndEquippingWeapons);


	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AGUNMANCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGUNMANCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AGUNMANCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AGUNMANCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AGUNMANCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AGUNMANCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AGUNMANCharacter::OnResetVR);

	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("FaceButtonRight", EKeys::Gamepad_FaceButton_Right));
	PlayerInputComponent->BindAction("FaceButtonRight", IE_Pressed, this, &AGUNMANCharacter::PressFaceButtonRight);
	PlayerInputComponent->BindAction("FaceButtonRight", IE_Released, this, &AGUNMANCharacter::ReleaseFaceButtonRight);

}

void AGUNMANCharacter::TimelineStep(float value)
{
	UE_LOG(LogTemp, Log, TEXT("Timeline(%.2f) : val = %.2f"), Timer, value);
	float NewSpeed = FMath::Lerp(StartSpeed, EndSpeed, value);
	SetMaxWalkSpeed(value);
}

void AGUNMANCharacter::OnFire()
{
	// FirstPerson だったら
	if (isFP)
	{
		// 腕のメッシュのアニメーションオブジェクトを取得
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();

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
			UWorld* const World = GetWorld();
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

		FHitResult HitResult;
		FVector StartLocation = ThirdPersonCamera->GetComponentLocation();
		FVector EndLocation = StartLocation + ThirdPersonCamera->GetForwardVector() * 10000.0f;
		FCollisionQueryParams TraceParams(FName(TEXT("LineTraceByChannel")), true, this);
		TraceParams.bTraceComplex = false; // 詳細な衝突判定を行うか
		TraceParams.AddIgnoredActor(this); // このアクターを無視する

		// ライントレースを実行
		bool bHit = GetWorld()->LineTraceSingleByChannel(
			HitResult,		// ヒット結果を格納する変数
			StartLocation,	// ラインの始点
			EndLocation,	// ラインの終点（前方向に1000cm移動）
			ECC_Visibility,	// 可視性チャンネル
			TraceParams		// ライントレースのパラメータ設定
		);

		if (bHit)
		{
			// ヒットした場所の情報を使用
			FVector HitLocation = HitResult.Location;
			UGameplayStatics::SpawnEmitterAtLocation(
				this,					// エミッターを生成するアクター（GUNMANCharacter自身）
				WeaponEmitter,				// エミッターテンプレート
				HitLocation,			// エミッターの生成位置
				FRotator::ZeroRotator,	// エミッターの初期回転
				true,					// 自動でエミッターをアタッチするか
				EPSCPoolMethod::None,	// エミッタープールの方法
				true					// エミッターをワールドスペースで生成するか
			);

			// ヒットした敵の情報を使用
			TWeakObjectPtr<AActor> HitEnemy = HitResult.Actor;
			// アクターが敵だったら
			AAIEnemy* Enemy = Cast<AAIEnemy>(HitEnemy);
			if (Enemy)
			{
				AController* EventInstigator = NULL;
				AActor* DamageCauser = NULL;
				TSubclassOf<UDamageType> DamageTypeClass = NULL;
				if (Enemy->Health > 0.0f)
				{
					// 10 ダメージを与える
					UGameplayStatics::ApplyDamage(Enemy, 10.0f, EventInstigator, DamageCauser, DamageTypeClass);
					if (Enemy->Health <= 0.0f)
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

		FHitResult HitResult;
		FVector StartLocation = ThirdPersonCamera->GetComponentLocation();
		FVector EndLocation = StartLocation + ThirdPersonCamera->GetForwardVector() * 10000.0f;
		FCollisionQueryParams TraceParams(FName(TEXT("LineTraceByChannel")), true, this);
		TraceParams.bTraceComplex = false; // 詳細な衝突判定を行うか
		TraceParams.AddIgnoredActor(this); // このアクターを無視する

		// ライントレースを実行
		bool bHit = GetWorld()->LineTraceSingleByChannel(
			HitResult,		// ヒット結果を格納する変数
			StartLocation,	// ラインの始点
			EndLocation,	// ラインの終点（前方向に10000cm移動）
			ECC_Visibility,	// 可視性チャンネル
			TraceParams		// ライントレースのパラメータ設定
		);

		// 色を付ける（ライントレースだけだと色が出なかった）
		//DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 0.3f);

		// 指定された場合、サウンドを再生
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
				true,					// 自動でエミッターをアタッチするか
				EPSCPoolMethod::None,	// エミッタープールの方法
				true					// エミッターをワールドスペースで生成するか
			);

			// ヒットした敵の情報を使用
			TWeakObjectPtr<AActor> HitEnemy = HitResult.Actor;
			// アクターが敵だったら
			AAIEnemy* Enemy = Cast<AAIEnemy>(HitEnemy);
			if (Enemy)
			{
				AController* EventInstigator = NULL;
				AActor* DamageCauser = NULL;
				TSubclassOf<UDamageType> DamageTypeClass = NULL;
				if (Enemy->Health > 0.0f)
				{
					// 10 ダメージを与える
					UGameplayStatics::ApplyDamage(Enemy, 10.0f, EventInstigator, DamageCauser, DamageTypeClass);
					if (Enemy->Health <= 0.0f)
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
	if (isA)
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
		TArray<AActor*> WeaponList; // レベルから武器を探して取得
		UGameplayStatics::GetAllActorsOfClass(this, ARifle::StaticClass(), WeaponList);
		for (AActor* SearchActor : WeaponList)
		{
			ARifle* NewActor = Cast<ARifle>(SearchActor);
			NewActor->SetActorHiddenInGame(true);
		}

		// FirstPerson のメッシュと銃を表示
		Mesh1P->SetVisibility(true);
		FP_Gun->SetVisibility(true);

		// FirstPerson だったら
		isFP = true;

		isA = false;
	}
	else
	{
		// ThirdPerson
		// ThirdPerson のカメラをオン
		ThirdPersonCamera->SetActive(true);
		FirstPersonCamera->SetActive(false);

		bUseControllerRotationYaw = false;

		// Character クラスの Mesh(ThirdPersonのメッシュ) を取得
		USkeletalMeshComponent* MeshComponent = GetMesh();

		// ThirdPerson のメッシュを表示
		MeshComponent->SetVisibility(true);
		Weapon->SetVisibility(false);
		TArray<AActor*> WeaponList; // レベルから武器を探して取得
		UGameplayStatics::GetAllActorsOfClass(this, ARifle::StaticClass(), WeaponList);
		for (AActor* SearchActor : WeaponList)
		{
			ARifle* NewActor = Cast<ARifle>(SearchActor);
			NewActor->SetActorHiddenInGame(false);
		}

		// FirstPerson のメッシュと銃を非表示
		Mesh1P->SetVisibility(false);
		FP_Gun->SetVisibility(false);

		// ThirdPerson だったら
		isFP = false;

		isA = true;
	}
}

void AGUNMANCharacter::SwitchingAndEquippingWeapons()
{
}

void AGUNMANCharacter::OnResetVR()
{
	// If GUNMAN is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in GUNMAN.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AGUNMANCharacter::StartJump()
{
	// ジャンプする
	Jump();

	// ジャンプしたか
	JumpButtonDown = CanJump();

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

void AGUNMANCharacter::StartCrouching()
{
	// しゃがむ
	//CrouchButtonDown = true;
}

void AGUNMANCharacter::StopCrouching()
{
	// 起きる
	//CrouchButtonDown = false;
}

void AGUNMANCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void AGUNMANCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void AGUNMANCharacter::HandleAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	// ダメージ分、体力を減らす
	Health -= Damage;
	// 体力がなくなったら
	if (Health <= 0.0f)
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
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AGUNMANCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AGUNMANCharacter::PressFaceButtonRight()
{
	UE_LOG(LogTemp, Log, TEXT("PressFaceButtonRight"));
}
void AGUNMANCharacter::ReleaseFaceButtonRight()
{
	UE_LOG(LogTemp, Log, TEXT("ReleaseFaceButtonRight"));
}
