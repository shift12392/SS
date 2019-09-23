// Fill out your copyright notice in the Description page of Project Settings.


#include "SSGameInstance.h"

#include "Kismet/GameplayStatics.h"
#include "SSSaveGame.h"



USSGameInstance::USSGameInstance()
	: SaveSlot(TEXT("SaveGame"))
	, SaveUserIndex(0)
	, bSavingEnabled(false)
	, bCurrentlySaving(false)
	, bPendingSaveRequested(false)
{}



void USSGameInstance::AddDefaultInventory(USSSaveGame* SaveGame, bool bRemoveExtra)
{
	// If we want to remove extra, clear out the existing inventory
	if (bRemoveExtra)
	{
		SaveGame->InventoryData.Reset();
	}

	// Now add the default inventory, this only adds if not already in hte inventory
	for (const TPair<FPrimaryAssetId, FSSItemData>& Pair : DefaultInventory)
	{
		if (!SaveGame->InventoryData.Contains(Pair.Key))
		{
			SaveGame->InventoryData.Add(Pair.Key, Pair.Value);
		}
	}
}

bool USSGameInstance::IsValidItemSlot(FSSItemSlot ItemSlot) const
{
	if (ItemSlot.IsValid())
	{
		const int32* FoundCount = ItemSlotsPerType.Find(ItemSlot.ItemType);

		if (FoundCount)
		{
			return ItemSlot.SlotNumber < *FoundCount;
		}
	}
	return false;
}

USSSaveGame* USSGameInstance::GetCurrentSaveGame()
{
	return CurrentSaveGame;
}

void USSGameInstance::SetSavingEnabled(bool bEnabled)
{
	bSavingEnabled = bEnabled;
}

bool USSGameInstance::LoadOrCreateSaveGame()
{
	USSSaveGame* LoadedSave = nullptr;

	if (UGameplayStatics::DoesSaveGameExist(SaveSlot, SaveUserIndex) && bSavingEnabled)
	{
		LoadedSave = Cast<USSSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlot, SaveUserIndex));
	}

	return HandleSaveGameLoaded(LoadedSave);
}

bool USSGameInstance::HandleSaveGameLoaded(USaveGame * SaveGameObject)
{
	bool bLoaded = false;

	if (!bSavingEnabled)
	{
		// If saving is disabled, ignore passed in object
		SaveGameObject = nullptr;
	}

	// Replace current save, old object will GC out
	CurrentSaveGame = Cast<USSSaveGame>(SaveGameObject);

	if (CurrentSaveGame)
	{
		// Make sure it has any newly added default inventory
		AddDefaultInventory(CurrentSaveGame, false);
		bLoaded = true;
	}
	else
	{
		// This creates it on demand
		CurrentSaveGame = Cast<USSSaveGame>(UGameplayStatics::CreateSaveGameObject(USSSaveGame::StaticClass()));

		AddDefaultInventory(CurrentSaveGame, true);
	}

	OnSaveGameLoaded.Broadcast(CurrentSaveGame);
	OnSaveGameLoadedNative.Broadcast(CurrentSaveGame);

	return bLoaded;
}

void USSGameInstance::GetSaveSlotInfo(FString & SlotName, int32 & UserIndex) const
{
	SlotName = SaveSlot;
	UserIndex = SaveUserIndex;
}

bool USSGameInstance::WriteSaveGame()
{
	if (bSavingEnabled)
	{
		if (bCurrentlySaving)
		{
			// Schedule another save to happen after current one finishes. We only queue one save
			bPendingSaveRequested = true;
			return true;
		}

		// Indicate that we're currently doing an async save
		bCurrentlySaving = true;

		// This goes off in the background
		UGameplayStatics::AsyncSaveGameToSlot(GetCurrentSaveGame(), SaveSlot, SaveUserIndex, FAsyncSaveGameToSlotDelegate::CreateUObject(this, &USSGameInstance::HandleAsyncSave));
		return true;
	}
	return false;
}

void USSGameInstance::ResetSaveGame()
{
	// Call handle function with no loaded save, this will reset the data
	HandleSaveGameLoaded(nullptr);
}

void USSGameInstance::HandleAsyncSave(const FString& SlotName, const int32 UserIndex, bool bSuccess)
{
	ensure(bCurrentlySaving == true);
	bCurrentlySaving = false;

	if (bPendingSaveRequested)
	{
		bPendingSaveRequested = false;
		WriteSaveGame();
	}
}


