// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "LearnSlateStandaloneCommands.h"

#define LOCTEXT_NAMESPACE "FLearnSlateStandaloneModule"

void FLearnSlateStandaloneCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "LearnSlateStandalone", "Bring up LearnSlateStandalone window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
