// Fill out your copyright notice in the Description page of Project Settings.


#include "EndlessRun/Characters/RunCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

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

	// Enhanced Input のアセットをロード
	static ConstructorHelpers::FObjectFinder<class UInputMappingContext> MappingContextFinder(TEXT("/Game/EndlessRun/Input/IMC_Default.IMC_Default"));
	if (MappingContextFinder.Succeeded())
	{
		DefaultMappingContext = MappingContextFinder.Object;
	}

}

// Called when the game starts or when spawned
void ARunCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARunCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARunCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

