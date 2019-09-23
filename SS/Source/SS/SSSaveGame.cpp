// Fill out your copyright notice in the Description page of Project Settings.


#include "SSSaveGame.h"
#include "SSTypes.h"




void USSSaveGame::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);

	if (Ar.IsLoading() && SavedDataVersion != ESSSaveGameVersion::LatestVersion)
	{
		if (SavedDataVersion < ESSSaveGameVersion::AddedItemData)
		{
			// Convert from list to item data map
			for (const FPrimaryAssetId& ItemId : InventoryItems_DEPRECATED)
			{
				InventoryData.Add(ItemId, FSSItemData(1, 1));
			}

			InventoryItems_DEPRECATED.Empty();
		}

		SavedDataVersion = ESSSaveGameVersion::LatestVersion;
	}
}
