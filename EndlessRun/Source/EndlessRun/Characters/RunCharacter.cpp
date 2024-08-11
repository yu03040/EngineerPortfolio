// Fill out your copyright notice in the Description page of Project Settings.


#include "EndlessRun/Characters/RunCharacter.h"
#include "EndlessRun/UMG/PauseMenu/PauseMenu.h"
#include "EndlessRun/UMG/GamePlaying/CountDown.h"
#include "EndlessRun/GameModes/RunGameMode.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "UObject/ConstructorHelpers.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "TimerManager.h"

// Sets default values
ARunCharacter::ARunCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// collision capsule のサイズを設定する
	GetCapsuleComponent()->InitCapsuleSize(35.f, 90.0f);

	// コントローラーが回転しても回転させない
	// カメラに影響を与えるだけにする
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// キャラクターの動きを設定する
	GetCharacterMovement()->bOrientRotationToMovement = true; //    ͂  ꂽ     ɃL     N ^ [      ...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...   ̉ ]    
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;

	// カメラブームの作成
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character
	CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 100.0f);
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->SetRelativeLocation(FVector(0.0f, 0.0f, 8.5f));

	// カメラコンポーネントの作成
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Emitter のアセットをロード
	static ConstructorHelpers::FObjectFinder<UParticleSystem> EmitterFinder(TEXT("/Game/StarterContent/Particles/P_Explosion.P_Explosion"));
	if (EmitterFinder.Succeeded())
	{
		Emitter = EmitterFinder.Object;
	}

	// サウンドのアセットをロード
	static ConstructorHelpers::FObjectFinder<USoundBase> SoundFinder(TEXT("/Game/StarterContent/Audio/Explosion_Cue.Explosion_Cue"));
	if (SoundFinder.Succeeded())
	{
		ExplosionSound = SoundFinder.Object;
	}

	// Enhanced Input のアセットをロード
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> MappingContextFinder(TEXT("/Game/EndlessRun/Input/IMC_Default.IMC_Default"));
	if (MappingContextFinder.Succeeded())
	{
		DefaultMappingContext = MappingContextFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> PauseActionFinder(TEXT("/Game/EndlessRun/Input/Actions/IA_Pause.IA_Pause"));
	if (PauseActionFinder.Succeeded())
	{
		PauseAction = PauseActionFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> JumpActionFinder(TEXT("/Game/EndlessRun/Input/Actions/IA_Jump.IA_Jump"));
	if (JumpActionFinder.Succeeded())
	{
		JumpAction = JumpActionFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> MoveActionFinder(TEXT("/Game/EndlessRun/Input/Actions/IA_Move.IA_Move"));
	if (MoveActionFinder.Succeeded())
	{
		MoveAction = MoveActionFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> TurnLeftActionActionFinder(TEXT("/Game/EndlessRun/Input/Actions/IA_TurnLeft.IA_TurnLeft"));
	if (TurnLeftActionActionFinder.Succeeded())
	{
		TurnLeftAction = TurnLeftActionActionFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> TurnRightActionFinder(TEXT("/Game/EndlessRun/Input/Actions/IA_TurnRight.IA_TurnRight"));
	if (TurnRightActionFinder.Succeeded())
	{
		TurnRightAction = TurnRightActionFinder.Object;
	}
}

// Called when the game starts or when spawned
void ARunCharacter::BeginPlay()
{
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

	// カウントダウンタイマーのために Tick を停止
	TObjectPtr<ARunGameMode> GameMode = Cast<ARunGameMode>(UGameplayStatics::GetGameMode(this));
	TObjectPtr<ACharacter> Character = UGameplayStatics::GetPlayerCharacter(this, 0);
	bool bEnabled = false;
	GameMode->SetActorTickEnabled(bEnabled);
	Character->SetActorTickEnabled(bEnabled);

	// WidgetBlueprintのClassを取得する
	FString Path = TEXT("/Game/EndlessRun/Blueprints/UMG/WBP_CountDown_v2.WBP_CountDown_v2_C");
	TSubclassOf<UUserWidget> WidgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*Path)).LoadSynchronous();

	// WidgetClassとPlayerControllerが取得できたか判定する
	if (WidgetClass && PlayerController)
	{
		TObjectPtr<UCountDown> CountDownWidget = CreateWidget<UCountDown>(PlayerController, WidgetClass);
		if (CountDownWidget)
		{
			CountDownWidget->AddToViewport();
		}
	}
}

// Called every frame
void ARunCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 死ぬまで前方向に進み続ける
	if (bIsDead == false)
	{
		TurnCorner();

		// プレイヤーコントローラーから回転(Z(Yaw))を取得
		FRotator MoveRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

		// MoveRotation から前方向ベクトルを取得
		FVector WorldDirection = MoveRotation.RotateVector(FVector::ForwardVector);
		AddMovementInput(WorldDirection);
	}
}

// Called to bind functionality to input
void ARunCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	if (TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// ポーズメニューのバインド
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &ARunCharacter::PauseInput);

		// ジャンプのバインド
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ARunCharacter::StartJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ARunCharacter::StopJump);

		// 移動のバインド
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARunCharacter::MovementInput);

		// 方向転換のバインド
		EnhancedInputComponent->BindAction(TurnLeftAction, ETriggerEvent::Triggered, this, &ARunCharacter::TurnLeftMovement);
		EnhancedInputComponent->BindAction(TurnRightAction, ETriggerEvent::Triggered, this, &ARunCharacter::TurnRightMovement);
	}
}

void ARunCharacter::PauseInput()
{
	// WidgetBlueprintのClassを取得する
	FString Path = TEXT("/Game/EndlessRun/Blueprints/UMG/WBP_PauseMenu_v2.WBP_PauseMenu_v2_C");
	TSubclassOf<UUserWidget> WidgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*Path)).LoadSynchronous();

	// PlayerControllerを取得する
	TObjectPtr<APlayerController> PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	// WidgetClassとPlayerControllerが取得できたか判定する
	if (WidgetClass && PlayerController)
	{
		TObjectPtr<UPauseMenu> PauseMenuWidget = CreateWidget<UPauseMenu>(PlayerController, WidgetClass);
		if (PauseMenuWidget)
		{
			PauseMenuWidget->AddToViewport();
		}
	}
}

void ARunCharacter::StartJump()
{
	Jump();
}

void ARunCharacter::StopJump()
{
	StopJumping();
}

void ARunCharacter::MovementInput(const FInputActionValue& Value)
{
	if (Controller != nullptr)
	{
		FVector2D ActionValue = Value.Get<FVector2D>();

		// プレイヤーコントローラーから回転(X(Roll), Z(Yaw))を取得
		FRotator MoveRotation(0.0f, Controller->GetControlRotation().Yaw, Controller->GetControlRotation().Roll);

		// 左右方向
		if (ActionValue.X != 0.0f)
		{
			// MoveRotation から右方向ベクトルを取得
			FVector WorldDirection = MoveRotation.RotateVector(FVector::RightVector);
			AddMovementInput(WorldDirection, ActionValue.X);
		}
	}
}

void ARunCharacter::TurnLeftMovement()
{
	if (bCanTurn == true)
	{
		// 左に曲がるので -90 度
		DesiredRotation = UKismetMathLibrary::ComposeRotators(DesiredRotation, FRotator(0.0f, -90.0f, 0.0f));
		bCanTurn = false;
	}
}

void ARunCharacter::TurnRightMovement()
{
	if (bCanTurn == true)
	{
		// 右に曲がるので 90 度
		DesiredRotation = UKismetMathLibrary::ComposeRotators(DesiredRotation, FRotator(0.0f, 90.0f, 0.0f));
		bCanTurn = false;
	}
}

void ARunCharacter::TurnCorner()
{
	// 希望の角度ではなかったら希望の角度までブレンドしていく
	bool bUnwantedRotation = UKismetMathLibrary::NotEqual_RotatorRotator(DesiredRotation, GetControlRotation(), 1.0E-4F);
	if (bUnwantedRotation == true)
	{
		TObjectPtr<APlayerController> PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		float InterpSpeed = 10.0f;
		FRotator BlendRotation = UKismetMathLibrary::RInterpTo(GetControlRotation(), DesiredRotation, UGameplayStatics::GetWorldDeltaSeconds(this), InterpSpeed);
		PlayerController->SetControlRotation(BlendRotation);
	}
}

void ARunCharacter::Death()
{
	// 死んだらオンにする
	bIsDead = true;

	// 入力を止める
	TObjectPtr<APlayerController> PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	DisableInput(PlayerController);

	// 爆発エフェクトを発生させる
	if (bIsEffect == false)
	{
		FTransform SpawnTransform(FRotator(0.0f, 0.0f, 0.0f), FVector(GetActorLocation()), FVector(1.0f, 1.0f, 1.0f));
		bool bAutoDestroy = true;
		bool bAutoActiveSystem = true;
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Emitter, SpawnTransform, bAutoDestroy, EPSCPoolMethod::None, bAutoActiveSystem);

		// 爆発音を再生させる
		UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());

		bIsEffect = true;
	}

	// プレイヤーを非表示にする
	GetMesh()->SetVisibility(false, false);

	// 1 秒後にリザルトを表示する
	ARunGameMode* GameMode = Cast<ARunGameMode>(UGameplayStatics::GetGameMode(this));
	float DelayTime = 1.0f;
	if (GameMode)
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, GameMode, &ARunGameMode::ShowResult, DelayTime, false);
	}

}
