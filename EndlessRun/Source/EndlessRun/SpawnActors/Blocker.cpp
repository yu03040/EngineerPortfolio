// Fill out your copyright notice in the Description page of Project Settings.


#include "EndlessRun/SpawnActors/Blocker.h"
#include "EndlessRun/Characters/RunCharacter.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"

// Sets default values
ABlocker::ABlocker()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// SceneComponent を作成する
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = DefaultSceneRoot;

	// BoxComponent を作成する
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	Box->SetBoxExtent(FVector(32.0f, 32.0f, 200.0f));
	Box->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
	Box->SetupAttachment(RootComponent);
	Box->OnComponentBeginOverlap.AddDynamic(this, &ABlocker::OnOverlapBegin);

	// NiagaraComponent を作成する
	Niagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	Niagara->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
	Niagara->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABlocker::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABlocker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	int TickCount = 10;
	// 雷を速くバチバチさせたいので、事前に 10Tick 分シミュレーション を進める
	Niagara->AdvanceSimulation(TickCount, DeltaTime);
}

void ABlocker::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	TObjectPtr<ARunCharacter> Player = Cast<ARunCharacter>(OtherActor);
	if (Player)
	{
		// プレイヤーが死んだことにする
		Player->Death();
	}
}

