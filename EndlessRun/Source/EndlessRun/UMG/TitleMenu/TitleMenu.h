// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Sound/SoundBase.h"
#include "TitleMenu.generated.h"

class UImage;
class UTextBlock;
class UButton;
class UWidgetAnimation;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameStartedDelegate);

/**
 * 
 */
UCLASS()
class ENDLESSRUN_API UTitleMenu : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UImage> Image_Background;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> TextBlock_EndlessRun;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UButton> Button_Start;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> TextBlock_GameStart;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UButton> Button_End;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> TextBlock_GameEnd;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient, meta = (BindWidgetAnim), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetAnimation> WidgetAnimation_LogoFrameIn;

protected:
	// クリック音
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	TObjectPtr<USoundBase> ClickSound;

public:
	// ゲームを開始する呼び出し
	UPROPERTY(BlueprintAssignable)
	FGameStartedDelegate OnGameStarted;

protected:
	virtual void NativeConstruct() override;

public:
	// Button_Start の OnClicked イベントに関連づける関数
	UFUNCTION()
	void OnButtonStartClicked();

	// Button_End の OnClicked イベントに関連づける関数
	UFUNCTION()
	void OnButtonEndClicked();

	void StartGame();

	void EndGame();
};
