// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "BatteryCollectorCharacter.h"
#include "Pickup.h"
#include "BatteryPickup.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Runtime/Engine/Classes/Components/SphereComponent.h"

//////////////////////////////////////////////////////////////////////////
// ABatteryCollectorCharacter

ABatteryCollectorCharacter::ABatteryCollectorCharacter()
{
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
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	// CollectionSphere の設定
	CollectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollectionSphere"));

	// CollectionSphere をルートの子に設定
	CollectionSphere->SetupAttachment(RootComponent);

	// Sphere の半径を設定
	CollectionSphere->SetSphereRadius(200.0f);

	// バッテリデフォルト値設定
	InitialPower = 2000.0f;
	CharacterPower = InitialPower;

	// Power に依存する速度の設定
	SpeedFactor = 0.75f;
	BaseSpeed = 10.0f;
}

//////////////////////////////////////////////////////////////////////////
// Input

void ABatteryCollectorCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ABatteryCollectorCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABatteryCollectorCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ABatteryCollectorCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ABatteryCollectorCharacter::LookUpAtRate);

	// アイテム収集ボタンのバイディング
	PlayerInputComponent->BindAction("Collect", IE_Released, this, &ABatteryCollectorCharacter::CollectPickups);
}

float ABatteryCollectorCharacter::GetInitialPower()
{
	return InitialPower;
}

float ABatteryCollectorCharacter::GetCharacterPower()
{
	return CharacterPower;
}

void ABatteryCollectorCharacter::UpdateCharacterPower(float PowerChange)
{
	// バッテリー値更新
	CharacterPower = CharacterPower + PowerChange;

	// キャラクターのスピードの更新
	GetCharacterMovement()->MaxWalkSpeed = BaseSpeed + (SpeedFactor * CharacterPower);

	// バッテリー値によって色を変える
	PowerChangeEffect();
}

void ABatteryCollectorCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ABatteryCollectorCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ABatteryCollectorCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ABatteryCollectorCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
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

void ABatteryCollectorCharacter::CollectPickups()
{
	// アイテム収集範囲内に入っているオブジェクトを取得する
	TArray<AActor*> CollectedActors;
	CollectionSphere->GetOverlappingActors(CollectedActors);

	float CollectedPower = 0;

	for (int32 isCollected = 0; isCollected < CollectedActors.Num(); isCollected++)
	{
		APickup* const TestPickup = Cast<APickup>(CollectedActors[isCollected]);

		// キャストが成功し、破棄されておらず、アクティブな時だけ実行
		if (TestPickup && !TestPickup->IsPendingKill() && TestPickup->IsActive())
		{
			TestPickup->WasCollected();

			ABatteryPickup* const TestBattery = Cast<ABatteryPickup>(TestPickup);
			if (TestBattery)
			{
				CollectedPower += TestBattery->GetPower();
			}
			TestPickup->SetActive(false);
		}
	}

	// 回収した分のバッテリーを反映させる
	if (CollectedPower > 0)
	{
		UpdateCharacterPower(CollectedPower);
	}
}