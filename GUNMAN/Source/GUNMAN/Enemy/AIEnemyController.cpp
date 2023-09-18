// Fill out your copyright notice in the Description page of Project Settings.


#include "AIEnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GamePlayStatics.h"

void AAIEnemyController::UpdateTargetActorKey_Implementation(AActor* TargetActor)
{
	// BlackboardComponentの取得
	UBlackboardComponent* BlackboardComp = GetBlackboardComponent();

    if (BlackboardComp)
    {
        // オブジェクトをセットするためのキー（キーID）を取得
        FName ObjectKey = FName("TargetActor");

        // SetValueAsObjectを使用してオブジェクトをセット
        BlackboardComp->SetValueAsObject(ObjectKey, TargetActor);
    }
}

void AAIEnemyController::UpdateHasLineOfSightKey_Implementation(bool HasLineOfSight)
{
    // BlackboardComponentの取得
    UBlackboardComponent* BlackboardComp = GetBlackboardComponent();

    if (BlackboardComp)
    {
        // オブジェクトをセットするためのキー（キーID）を取得
        FName ObjectKey = FName("HasLIneOfSight");

        // SetValueAsObjectを使用してオブジェクトをセット
        BlackboardComp->SetValueAsBool(ObjectKey, HasLineOfSight);
    }
}

void AAIEnemyController::BeginPlay()
{
    Super::BeginPlay();

    // プレイヤーキャラクター（ACharacter）を取得する
    Player = UGameplayStatics::GetPlayerCharacter(this->GetWorld(), 0);
}
