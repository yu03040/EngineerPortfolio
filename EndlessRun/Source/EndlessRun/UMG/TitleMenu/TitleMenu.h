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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio", meta = (AllowPrivateAccess = "true"))
	TObjectPtr <USoundBase> SoundObj;

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void OnButtonStartClicked();

	UFUNCTION()
	void OnButtonEndClicked();

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameStartedDelegate);

	UPROPERTY(BlueprintAssignable)
	FGameStartedDelegate OnGameStarted;
};
