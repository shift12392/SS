// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/SSItem.h"
#include "SSWeaponItem.generated.h"

/**
 * 
 */
UCLASS()
class SS_API USSWeaponItem : public USSItem
{
	GENERATED_BODY()

public:
	USSWeaponItem()
	{
		ItemType = USSAssetManager::WeaponItemType;
	}
	
};
