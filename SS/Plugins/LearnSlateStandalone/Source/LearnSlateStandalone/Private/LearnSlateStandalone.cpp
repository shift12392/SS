// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "LearnSlateStandalone.h"
#include "LearnSlateStandaloneStyle.h"
#include "LearnSlateStandaloneCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"

static const FName LearnSlateStandaloneTabName("LearnSlateStandalone");

#define LOCTEXT_NAMESPACE "FLearnSlateStandaloneModule"

void FLearnSlateStandaloneModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FLearnSlateStandaloneStyle::Initialize();
	FLearnSlateStandaloneStyle::ReloadTextures();

	FLearnSlateStandaloneCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FLearnSlateStandaloneCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FLearnSlateStandaloneModule::PluginButtonClicked),
		FCanExecuteAction());
		
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	
	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FLearnSlateStandaloneModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}
	
	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FLearnSlateStandaloneModule::AddToolbarExtension));
		
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(LearnSlateStandaloneTabName, FOnSpawnTab::CreateRaw(this, &FLearnSlateStandaloneModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FLearnSlateStandaloneTabTitle", "LearnSlateStandalone"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FLearnSlateStandaloneModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FLearnSlateStandaloneStyle::Shutdown();

	FLearnSlateStandaloneCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(LearnSlateStandaloneTabName);
}

TSharedRef<SDockTab> FLearnSlateStandaloneModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FLearnSlateStandaloneModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("LearnSlateStandalone.cpp"))
		);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(WidgetText)
			]
		];
}

void FLearnSlateStandaloneModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->InvokeTab(LearnSlateStandaloneTabName);
}

void FLearnSlateStandaloneModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FLearnSlateStandaloneCommands::Get().OpenPluginWindow);
}

void FLearnSlateStandaloneModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FLearnSlateStandaloneCommands::Get().OpenPluginWindow);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FLearnSlateStandaloneModule, LearnSlateStandalone)