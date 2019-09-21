// Fill out your copyright notice in the Description page of Project Settings.


#include "SSItem.h"

bool USSItem::IsConsumable() const
{
	if (MaxCount <= 0)
	{
		return true;
	}
	return false;
}

FString USSItem::GetIdentifierString() const
{
	return GetPrimaryAssetId().ToString();
}

FPrimaryAssetId USSItem::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(ItemType,GetFName());
}
