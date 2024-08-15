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

AItem::AItem()
{
	// 毎フレームTick()を呼び出すように設定
	PrimaryActorTick.bCanEverTick = true;

	// SceneComponent を作成する
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = DefaultSceneRoot;

	// StaticMeshComponentを作成する
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, -90.0f));
	StaticMesh->SetupAttachment(RootComponent);

	// StaticMesh の OnComponentBeginOverlap に「OnOverlapBegin」を関連づける
	StaticMesh->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnOverlapBegin);

	// PointLightComponent を作成する
	PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLightComponent"));
	PointLight->Intensity = 40.0f;
	PointLight->IntensityUnits = ELightUnits::Lumens;
	PointLight->SetupAttachment(StaticMesh);

	// RotatingMovementComponent を作成する
	RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovementComponent"));
	RotatingMovement->RotationRate = FRotator(0.0f, 180.0f, 0.0f);
	RotatingMovement->SetUpdatedComponent(RootComponent);

	// サウンドのロード
	static ConstructorHelpers::FObjectFinder<USoundBase> SoundFinder(TEXT("/Game/FreeAssets/SoundEffects/8bit_acquisition_8.8bit_acquisition_8"));
	if (SoundFinder.Succeeded())
	{
		ItemSound = SoundFinder.Object;
	}
}

void AItem::BeginPlay()
{
	Super::BeginPlay();

}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	TObjectPtr<ARunCharacter> Player = Cast<ARunCharacter>(OtherActor);
	if (Player)
	{
		if (Player->bIsDead == false)
		{
			TObjectPtr<ARunGameMode> GameMode = Cast<ARunGameMode>(UGameplayStatics::GetGameMode(this));
			if (GameMode)
			{
				// 取ったコインをカウントする
				GameMode->AddCoins();

				// コインの位置でサウンドを再生
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), ItemSound, GetActorLocation(), 0.2f);

				// 終わったらコインを消す
				Destroy();
			}
		}
	}
}

