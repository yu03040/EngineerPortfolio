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
	UUIEnemy* EnemyWidget = Cast<UUIEnemy>(Widget->GetUserWidgetObject());
	if (EnemyWidget)
	{
		TempWidget = EnemyWidget;
	}
}

void AAIEnemy::HandleAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	// ダメージ分、体力を減らす
	Health -= Damage;

	// 百分率に直し、ダメージ表示できるようにする
	Health /= 100.0f;
	TempWidget->HealthRate = Health;

	if (TempWidget->HealthRate <= 0.0f)
	{
		// 敵の動きを止める
		GetCharacterMovement()->DisableMovement();
		auto EnemyCapsuleComponent = GetCapsuleComponent();
		if (EnemyCapsuleComponent)
		{
			// コリジョンをオフ
			EnemyCapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			// 物理シュミレーションをオン
			GetMesh()->SetSimulatePhysics(true);
			// 2 秒たったら崩れる
			SetLifeSpan(2.0f);
			// 死んだら
			IsAlive = false;
		}
	}
	else
	{
		// 攻撃されたらランダムにアニメーションを再生する
		auto i = UKismetMathLibrary::RandomIntegerInRange(0, 3);
		PlayAnimMontage(HitAnimMontage[i]);
	}
}


void AAIEnemy::TargetLost()
{
	AAIEnemyController* AIEnemyController = CastChecked<AAIEnemyController>(EnemyController);
	if (AIEnemyController)
	{
		AIEnemyController->UpdateTargetActorKey_Implementation(NULL);
	}
}

// Called every frame
void AAIEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAIEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

