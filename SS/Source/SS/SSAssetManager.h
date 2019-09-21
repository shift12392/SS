// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "SSAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class SS_API USSAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:

	/** Static types for items */
	static const FPrimaryAssetType	PotionItemType;
	static const FPrimaryAssetType	SkillItemType;
	static const FPrimaryAssetType	TokenItemType;
	static const FPrimaryAssetType	WeaponItemType;
	
};
