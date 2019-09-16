// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/SSItem.h"
#include "SSPotionItem.generated.h"

/**
 * 
 */
UCLASS()
class SS_API USSPotionItem : public USSItem
{
	GENERATED_BODY()


public:
	/** Constructor */
	USSPotionItem()
	{
		ItemType = USSAssetManager::PotionItemType;
	}
	
};
