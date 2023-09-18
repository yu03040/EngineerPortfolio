// Copyright Epic Games, Inc. All Rights Reserved.

#include "GUNMANGameMode.h"
#include "GUNMANCharacter.h"
#include "GUNMANController.h"
#include "UMG/UITitle.h"
#include "UObject/ConstructorHelpers.h"

AGUNMANGameMode::AGUNMANGameMode() : Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprint/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	PlayerControllerClass = AGUNMANController::StaticClass();
}

void AGUNMANGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGUNMANGameMode::BeginPlay()
{
	Super::BeginPlay();
}
