// Fill out your copyright notice in the Description page of Project Settings.


#include "GUNMAN/Enemy/EnemyTargetPoint.h"
#include "Kismet/KismetMathLibrary.h"

AEnemyTargetPoint::AEnemyTargetPoint()
{
	EnemyCounter = 0;
	MaxEnemyCount = 0;
	BPEnemyPawnClass = NULL;
	SpawningIntervalTime = 0.0f;
	MinIntervalTime = 0.0f;
	MaxIntervalTime = 0.0f;
}

void AEnemyTargetPoint::BeginPlay()
{
	SpawnEnemy();
}

void AEnemyTargetPoint::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	FTimerManager& timerManager = GetWorldTimerManager();

	// Handleに登録されたTimerの解放
	timerManager.ClearTimer(TimerHandle);

	// このActorが所有するタイマーの解放
	timerManager.ClearAllTimersForObject(this);
}

void AEnemyTargetPoint::SpawnEnemy()
{
	if (EnemyCounter < MaxEnemyCount)
	{
		// 出現間隔設定
		SpawningIntervalTime = UKismetMathLibrary::RandomFloatInRange(MinIntervalTime, MaxIntervalTime);
		// 出現間隔時間分遅らせてスポーンする
		StartDelay(SpawningIntervalTime, GetWorld(), BPEnemyPawnClass, this->GetActorLocation(), this->GetActorRotation());

		EnemyCounter++;
		//SpawnEnemy();
	}

}

void AEnemyTargetPoint::SpawnAIClass(UWorld* World, TSubclassOf<APawn> PawnClass, const FVector& Location, const FRotator& Rotation)
{
	if (!World || !PawnClass)
	{
		return;
	}

	// パラメータを設定
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	// AIキャラクターをスポーン
	APawn* SpawnedPawn = World->SpawnActor<APawn>(PawnClass, Location, Rotation, SpawnParams);
	if (SpawnedPawn)
	{
		// AIコントローラを取得
		AAIEnemyController* AIController = Cast<AAIEnemyController>(SpawnedPawn->GetController());
		if (!AIController)
		{
			// AIコントローラがない場合は、新しく作成
			FString path = "/Game/Enemy/BP_AIEnemyController.BP_AIEnemyController_C";
			TSubclassOf<class AAIEnemyController> AIControllerClass = TSoftClassPtr<AAIEnemyController>(FSoftObjectPath(*path)).LoadSynchronous();
			AIController = World->SpawnActor<AAIEnemyController>(AIControllerClass, Location, Rotation, SpawnParams);
			if (AIController)
			{
				AIController->Possess(SpawnedPawn);
			}
		}
	}
}

void AEnemyTargetPoint::StartDelay(float DelayTime, UWorld* World, TSubclassOf<APawn> PawnClass, const FVector& Location, const FRotator& Rotation)
{
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this, FName(TEXT("SpawnAIClass")), World, PawnClass, Location, Rotation);

	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		TimerDelegate,
		DelayTime,
		true);
}
