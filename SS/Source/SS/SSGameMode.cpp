// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "SSGameMode.h"
#include "SSPlayerCharacter.h"
#include "Characters/SSPlayerController.h"
#include "UObject/ConstructorHelpers.h"

ASSGameMode::ASSGameMode()
{
	// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	//if (PlayerPawnBPClass.Class != NULL)
	//{
	//	DefaultPawnClass = PlayerPawnBPClass.Class;
	//}

	DefaultPawnClass = ASSPlayerCharacter::StaticClass();
	PlayerControllerClass = ASSPlayerController::StaticClass();

}
