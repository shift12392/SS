// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "LearnSlateStandaloneStyle.h"

class FLearnSlateStandaloneCommands : public TCommands<FLearnSlateStandaloneCommands>
{
public:

	FLearnSlateStandaloneCommands()
		: TCommands<FLearnSlateStandaloneCommands>(TEXT("LearnSlateStandalone"), NSLOCTEXT("Contexts", "LearnSlateStandalone", "LearnSlateStandalone Plugin"), NAME_None, FLearnSlateStandaloneStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};