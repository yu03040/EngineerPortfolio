// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "BaseMapScript.h"
#include "TitleMapScript.generated.h"

/**
 *
 */
UCLASS()
class GUNMAN_API ATitleMapScript : public ABaseMapScript
{
	GENERATED_BODY()

public:
	ATitleMapScript();

	/* ボタンの選択状態から非選択状態に遷移するときのボタンの色を変更する関数 */
	virtual void ChangeButtonColor() override;

	/* 選択されたボタンによって出力を変える関数 */
	virtual void UpdateOutputButton() override;

protected:
	virtual void BeginPlay() override;

private:
	/* UITitle の Ref */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	class UUITitle* UI_Title;
};
