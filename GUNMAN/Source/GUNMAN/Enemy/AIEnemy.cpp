// Fill out your copyright notice in the Description page of Project Settings.


#include "AIEnemy.h"
#include "AIEnemyController.h"
#include "GUNMAN/UMG/UIEnemy.h"
#include "Perception/AIPerceptionComponent.h"
#include "Kismet/GamePlayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AISenseConfig_Sight.h" // 視覚のセンサータイプ

// Sets default values
AAIEnemy::AAIEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Weapon を Mesh の子に
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh());

	// Widget を Mesh の子に
	Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	Widget->SetupAttachment(GetMesh());

	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));

	// スポーンされた敵に対してもAIを適用
	AutoPossessAI =  EAutoPossessAI::PlacedInWorldOrSpawned;

	IsAlive = true;
	CurrentHealth = 30.0f;
	MaxHealth = CurrentHealth;
	DeadHealth = 0.0f;
	EnemyATK = 10.0f;
	InLifeSpan = 2.0f;
}

// Called when the game starts or when spawned
void AAIEnemy::BeginPlay()
{
	Super::BeginPlay();

	// プレイヤーキャラクター（ACharacter）を取得する
	Player = UGameplayStatics::GetPlayerCharacter(this->GetWorld(), 0);
	// 敵のコントローラーを取得する
	EnemyController = GetController();

	// AnyDamageデリゲートをバインド
	OnTakeAnyDamage.AddDynamic(this, &AAIEnemy::HandleAnyDamage);

	// ウィジェットをもらう（ダメージ処理で使う）
	TObjectPtr<UUIEnemy> WidgetClass = Cast<UUIEnemy>(Widget->GetUserWidgetObject());
	if (WidgetClass)
	{
		EnemyWidget = WidgetClass;
		EnemyWidget->SetOwningEnemy(this);
	}
}

// Called every frame
void AAIEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAIEnemy::HandleAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	// ダメージ分、体力を減らす
	CurrentHealth -= Damage;

	if (CurrentHealth <= DeadHealth)
	{
		// 敵の動きを止める
		GetCharacterMovement()->DisableMovement();
		TObjectPtr<UCapsuleComponent> EnemyCapsuleComponent = GetCapsuleComponent();
		if (EnemyCapsuleComponent)
		{
			// コリジョンをオフ
			EnemyCapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			// 物理シュミレーションをオン
			GetMesh()->SetSimulatePhysics(true);
			// 指定された秒数が過ぎると崩れる
			SetLifeSpan(InLifeSpan);
			IsAlive = false;
		}
	}
	else
	{
		// 攻撃されたらランダムにアニメーションを再生する
		int32 i = UKismetMathLibrary::RandomIntegerInRange(0, 3);
		PlayAnimMontage(HitAnimMontage[i]);
	}
}


void AAIEnemy::TargetLost()
{
	TObjectPtr<AAIEnemyController> AIEnemyController = CastChecked<AAIEnemyController>(EnemyController);
	if (AIEnemyController)
	{
		AIEnemyController->UpdateTargetActorKey_Implementation(NULL);
	}
}

float AAIEnemy::GetCurrentHealth()
{
	return CurrentHealth;
}

float AAIEnemy::GetMaxHealth()
{
	return MaxHealth;
}

float AAIEnemy::GetHealthPercent()
{
	return CurrentHealth / MaxHealth;
}
