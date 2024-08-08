// Fill out your copyright notice in the Description page of Project Settings.


#include "EndlessRun/SpawnActors/Item.h"
#include "EndlessRun/Characters/RunCharacter.h"
#include "EndlessRun/GameModes/RunGameMode.h"
#include "Components/PointLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AItem::AItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// SceneComponent を作成する
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = DefaultSceneRoot;

	// StaticMeshComponentを作成する
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, -90.0f));
	StaticMesh->SetupAttachment(RootComponent);
	StaticMesh->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnOverlapBegin);

	// PointLightComponent を作成する
	PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLightComponent"));
	PointLight->Intensity = 1000.0f;
	PointLight->IntensityUnits = ELightUnits::Lumens;
	PointLight->SetupAttachment(StaticMesh);

	// RotatingMovementComponent を作成する
	RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovementComponent"));
	AddInstanceComponent(RotatingMovement);
	RotatingMovement->RotationRate = FRotator(0.0f, 180.0f, 0.0f);
	RotatingMovement->RegisterComponent();

	// サウンドのロード
	static ConstructorHelpers::FObjectFinder<USoundBase> SoundFinder(TEXT("/Engine/VREditor/Sounds/UI/Click_on_Button.Click_on_Button"));
	if (SoundFinder.Succeeded())
	{
		ItemSound = SoundFinder.Object;
	}
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	TObjectPtr<ARunCharacter> Player = Cast<ARunCharacter>(OtherActor);
	if (Player)
	{
		TObjectPtr<ARunGameMode> GameMode = Cast<ARunGameMode>(UGameplayStatics::GetGameMode(this));
		if (GameMode)
		{
			// 取ったコインをカウントする
			GameMode->AddCoins();

			// コインの位置でサウンドを再生
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ItemSound, GetActorLocation());

			// 終わったらコインを消す
			Destroy();
		}
	}
}

