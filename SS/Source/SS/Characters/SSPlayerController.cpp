// Fill out your copyright notice in the Description page of Project Settings.

#include "SSPlayerController.h"
#include "../SS.h"
#include "../SSGameInstance.h"
#include "Engine/World.h"
#include "SSAssetManager.h"
#include "../SSSaveGame.h"
#include "../Items/SSItem.h"


void ASSPlayerController::BeginPlay()
{

	LoadInventory();

	Super::BeginPlay();
}

const TMap<USSItem*, FSSItemData>& ASSPlayerController::GetInventoryDataMap() const
{
	// TODO: 在此处插入 return 语句

	return InventoryData;
}

const TMap<FSSItemSlot, USSItem*>& ASSPlayerController::GetSlottedItemMap() const
{
	// TODO: 在此处插入 return 语句
	return SlottedItems;
}

FOnInventoryItemChangedNative & ASSPlayerController::GetInventoryItemChangedDelegate()
{
	// TODO: 在此处插入 return 语句
	return OnInventoryItemChangedNative;
}

FOnSlottedItemChangedNative & ASSPlayerController::GetSlottedItemChangedDelegate()
{
	// TODO: 在此处插入 return 语句
	return OnSlottedItemChangedNative;
}

FOnInventoryLoadedNative & ASSPlayerController::GetInventoryLoadedDelegate()
{
	// TODO: 在此处插入 return 语句
	return OnInventoryLoadedNative;
}

bool ASSPlayerController::AddInventoryItem(USSItem* NewItem, int32 ItemCount, int32 ItemLevel, bool bAutoSlot)
{
	bool bChanged = false;
	if (!NewItem)
	{
		UE_LOG(LogSS, Warning, TEXT("AddInventoryItem: Failed trying to add null item!"));
		return false;
	}

	if (ItemCount <= 0 || ItemLevel <= 0)
	{
		UE_LOG(LogSS, Warning, TEXT("AddInventoryItem: Failed trying to add item %s with negative count or level!"), *NewItem->GetName());
		return false;
	}

	// Find current item data, which may be empty
	FSSItemData OldData;
	GetInventoryItemData(NewItem, OldData);

	// Find modified data
	FSSItemData NewData = OldData;
	NewData.UpdateItemData(FSSItemData(ItemCount, ItemLevel), NewItem->MaxCount, NewItem->MaxLevel);

	if (OldData != NewData)
	{
		// If data changed, need to update storage and call callback
		InventoryData.Add(NewItem, NewData);
		NotifyInventoryItemChanged(true, NewItem);
		bChanged = true;
	}

	if (bAutoSlot)
	{
		// Slot item if required
		bChanged |= FillEmptySlotWithItem(NewItem);
	}

	if (bChanged)
	{
		// If anything changed, write to save game
		SaveInventory();
		return true;
	}
	return false;
}

bool ASSPlayerController::RemoveInventoryItem(USSItem* RemovedItem, int32 RemoveCount)
{
	if (!RemovedItem)
	{
		UE_LOG(LogSS, Warning, TEXT("RemoveInventoryItem: Failed trying to remove null item!"));
		return false;
	}

	// Find current item data, which may be empty
	FSSItemData NewData;
	GetInventoryItemData(RemovedItem, NewData);

	if (!NewData.IsValid())
	{
		// Wasn't found
		return false;
	}

	// If RemoveCount <= 0, delete all
	if (RemoveCount <= 0)
	{
		NewData.ItemCount = 0;
	}
	else
	{
		NewData.ItemCount -= RemoveCount;
	}

	if (NewData.ItemCount > 0)
	{
		// Update data with new count
		InventoryData.Add(RemovedItem, NewData);
	}
	else
	{
		// Remove item entirely, make sure it is unslotted
		InventoryData.Remove(RemovedItem);

		for (TPair<FSSItemSlot, USSItem*>& Pair : SlottedItems)
		{
			if (Pair.Value == RemovedItem)
			{
				Pair.Value = nullptr;
				NotifySlottedItemChanged(Pair.Key, Pair.Value);
			}
		}
	}

	// If we got this far, there is a change so notify and save
	NotifyInventoryItemChanged(false, RemovedItem);

	SaveInventory();
	return true;
}

void ASSPlayerController::GetInventoryItems(TArray<USSItem*>& Items, FPrimaryAssetType ItemType)
{
	for (const TPair<USSItem*, FSSItemData>& Pair : InventoryData)
	{
		if (Pair.Key)
		{
			FPrimaryAssetId AssetId = Pair.Key->GetPrimaryAssetId();

			// Filters based on item type
			if (AssetId.PrimaryAssetType == ItemType || !ItemType.IsValid())
			{
				Items.Add(Pair.Key);
			}
		}
	}
}

bool ASSPlayerController::SetSlottedItem(FSSItemSlot ItemSlot, USSItem* Item)
{
	// Iterate entire inventory because we need to remove from old slot
	bool bFound = false;
	for (TPair<FSSItemSlot, USSItem*>& Pair : SlottedItems)
	{
		if (Pair.Key == ItemSlot)
		{
			// Add to new slot
			bFound = true;
			Pair.Value = Item;
			NotifySlottedItemChanged(Pair.Key, Pair.Value);
		}
		else if (Item != nullptr && Pair.Value == Item)
		{
			// If this item was found in another slot, remove it
			Pair.Value = nullptr;
			NotifySlottedItemChanged(Pair.Key, Pair.Value);
		}
	}

	if (bFound)
	{
		SaveInventory();
		return true;
	}

	return false;
}

int32 ASSPlayerController::GetInventoryItemCount(USSItem* Item) const
{
	const FSSItemData* FoundItem = InventoryData.Find(Item);

	if (FoundItem)
	{
		return FoundItem->ItemCount;
	}
	return 0;
}

bool ASSPlayerController::GetInventoryItemData(USSItem* Item, FSSItemData& ItemData) const
{
	const FSSItemData* FoundItem = InventoryData.Find(Item);

	if (FoundItem)
	{
		ItemData = *FoundItem;
		return true;
	}
	ItemData = FSSItemData(0, 0);
	return false;
}

USSItem* ASSPlayerController::GetSlottedItem(FSSItemSlot ItemSlot) const
{
	USSItem* const* FoundItem = SlottedItems.Find(ItemSlot);

	if (FoundItem)
	{
		return *FoundItem;
	}
	return nullptr;
}

void ASSPlayerController::GetSlottedItems(TArray<USSItem*>& Items, FPrimaryAssetType ItemType, bool bOutputEmptyIndexes)
{
	for (TPair<FSSItemSlot, USSItem*>& Pair : SlottedItems)
	{
		if (Pair.Key.ItemType == ItemType || !ItemType.IsValid())
		{
			Items.Add(Pair.Value);
		}
	}
}

void ASSPlayerController::FillEmptySlots()
{
	bool bShouldSave = false;
	for (const TPair<USSItem*, FSSItemData>& Pair : InventoryData)
	{
		bShouldSave |= FillEmptySlotWithItem(Pair.Key);
	}

	if (bShouldSave)
	{
		SaveInventory();
	}
}

bool ASSPlayerController::SaveInventory()
{
	UWorld* World = GetWorld();
	USSGameInstance* GameInstance = World ? World->GetGameInstance<USSGameInstance>() : nullptr;

	if (!GameInstance)
	{
		return false;
	}

	USSSaveGame* CurrentSaveGame = GameInstance->GetCurrentSaveGame();
	if (CurrentSaveGame)
	{
		// Reset cached data in save game before writing to it
		CurrentSaveGame->InventoryData.Reset();
		CurrentSaveGame->SlottedItems.Reset();

		for (const TPair<USSItem*, FSSItemData>& ItemPair : InventoryData)
		{
			FPrimaryAssetId AssetId;

			if (ItemPair.Key)
			{
				AssetId = ItemPair.Key->GetPrimaryAssetId();
				CurrentSaveGame->InventoryData.Add(AssetId, ItemPair.Value);
			}
		}

		for (const TPair<FSSItemSlot, USSItem*>& SlotPair : SlottedItems)
		{
			FPrimaryAssetId AssetId;

			if (SlotPair.Value)
			{
				AssetId = SlotPair.Value->GetPrimaryAssetId();
			}
			CurrentSaveGame->SlottedItems.Add(SlotPair.Key, AssetId);
		}

		// Now that cache is updated, write to disk
		GameInstance->WriteSaveGame();
		return true;
	}
	return false;
}


bool ASSPlayerController::LoadInventory()
{
	InventoryData.Reset();
	SlottedItems.Reset();

	// Fill in slots from game instance
	UWorld* World = GetWorld();
	USSGameInstance* GameInstance = World ? World->GetGameInstance<USSGameInstance>() : nullptr;

	if (!GameInstance)
	{
		return false;
	}

	// Bind to loaded callback if not already bound
	if (!GameInstance->OnSaveGameLoadedNative.IsBoundToObject(this))
	{
		GameInstance->OnSaveGameLoadedNative.AddUObject(this, &ASSPlayerController::HandleSaveGameLoaded);
	}

	for (const TPair<FPrimaryAssetType, int32>& Pair : GameInstance->ItemSlotsPerType)
	{
		for (int32 SlotNumber = 0; SlotNumber < Pair.Value; SlotNumber++)
		{
			SlottedItems.Add(FSSItemSlot(Pair.Key, SlotNumber), nullptr);
		}
	}

	USSSaveGame* CurrentSaveGame = GameInstance->GetCurrentSaveGame();
	USSAssetManager& AssetManager = USSAssetManager::Get();
	if (CurrentSaveGame)
	{
		// Copy from save game into controller data
		bool bFoundAnySlots = false;
		for (const TPair<FPrimaryAssetId, FSSItemData>& ItemPair : CurrentSaveGame->InventoryData)
		{
			USSItem* LoadedItem = AssetManager.ForceLoadItem(ItemPair.Key);

			if (LoadedItem != nullptr)
			{
				InventoryData.Add(LoadedItem, ItemPair.Value);
			}
		}

		for (const TPair<FSSItemSlot, FPrimaryAssetId>& SlotPair : CurrentSaveGame->SlottedItems)
		{
			if (SlotPair.Value.IsValid())
			{
				USSItem* LoadedItem = AssetManager.ForceLoadItem(SlotPair.Value);
				if (GameInstance->IsValidItemSlot(SlotPair.Key) && LoadedItem)
				{
					SlottedItems.Add(SlotPair.Key, LoadedItem);
					bFoundAnySlots = true;
				}
			}
		}

		if (!bFoundAnySlots)
		{
			// Auto slot items as no slots were saved
			FillEmptySlots();
		}

		NotifyInventoryLoaded();

		return true;
	}

	// Load failed but we reset inventory, so need to notify UI
	NotifyInventoryLoaded();

	return false;
}


bool ASSPlayerController::FillEmptySlotWithItem(USSItem* NewItem)
{
	// Look for an empty item slot to fill with this item
	FPrimaryAssetType NewItemType = NewItem->GetPrimaryAssetId().PrimaryAssetType;
	FSSItemSlot EmptySlot;
	for (TPair<FSSItemSlot, USSItem*>& Pair : SlottedItems)
	{
		if (Pair.Key.ItemType == NewItemType)
		{
			if (Pair.Value == NewItem)
			{
				// Item is already slotted
				return false;
			}
			else if (Pair.Value == nullptr && (!EmptySlot.IsValid() || EmptySlot.SlotNumber > Pair.Key.SlotNumber))
			{
				// We found an empty slot worth filling
				EmptySlot = Pair.Key;
			}
		}
	}

	if (EmptySlot.IsValid())
	{
		SlottedItems[EmptySlot] = NewItem;
		NotifySlottedItemChanged(EmptySlot, NewItem);
		return true;
	}

	return false;
}

void ASSPlayerController::NotifyInventoryItemChanged(bool bAdded, USSItem* Item)
{
	// Notify native before blueprint
	OnInventoryItemChangedNative.Broadcast(bAdded, Item);
	OnInventoryItemChanged.Broadcast(bAdded, Item);

	// Call BP update event
	InventoryItemChanged(bAdded, Item);
}

void ASSPlayerController::NotifySlottedItemChanged(FSSItemSlot ItemSlot, USSItem* Item)
{
	// Notify native before blueprint
	OnSlottedItemChangedNative.Broadcast(ItemSlot, Item);
	OnSlottedItemChanged.Broadcast(ItemSlot, Item);

	// Call BP update event
	SlottedItemChanged(ItemSlot, Item);
}

void ASSPlayerController::NotifyInventoryLoaded()
{
	// Notify native before blueprint
	OnInventoryLoadedNative.Broadcast();
	OnInventoryLoaded.Broadcast();
}

void ASSPlayerController::HandleSaveGameLoaded(USSSaveGame* NewSaveGame)
{
	LoadInventory();
}
