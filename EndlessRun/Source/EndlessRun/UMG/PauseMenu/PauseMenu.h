// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenu.generated.h"

class UTextBlock;
class UVerticalBox;
class UButton;
class USpacer;
class UBorder;

/**
 * 
 */
UCLASS()
class ENDLESSRUN_API UPauseMenu : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> TextBlock_PauseMenu;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UVerticalBox> VerticalBox;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UButton> Button_BackToTitle;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> TextBlock_BackToTitle;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpacer> Spacer_0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UButton> Button_Cancel;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> TextBlock_Cancel;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpacer> Spacer_1;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UButton> Button_QuitGame;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> TextBlock_QuitGame;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBorder> Border;

protected:
	// クリック音
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	TObjectPtr<USoundBase> ClickSound;

protected:
	virtual void NativeConstruct() override;

public:
	// Button_BackToTitle の OnClicked イベントに関連づける関数
	UFUNCTION()
	void OnButtonBackToTitleClicked();

	// Button_Cancel の OnClicked イベントに関連づける関数
	UFUNCTION()
	void OnButtonCancelClicked();

	// Button_QuitGame の OnClicked イベントに関連づける関数
	UFUNCTION()
	void OnButtonQuitGameClicked();

	UFUNCTION()
	void BackTitleAfterDelay();

	UFUNCTION()
	void CancelAfterDelay();

	UFUNCTION()
	void EndGameAfterDelay();
};
