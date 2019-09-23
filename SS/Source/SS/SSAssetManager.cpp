// Fill out your copyright notice in the Description page of Project Settings.


#include "SSAssetManager.h"
#include "SS.h"
#include "Items/SSItem.h"
#include "AbilitySystemGlobals.h"

const FPrimaryAssetType	USSAssetManager::PotionItemType = TEXT("Potion");
const FPrimaryAssetType	USSAssetManager::SkillItemType = TEXT("Skill");
const FPrimaryAssetType	USSAssetManager::TokenItemType = TEXT("Token");
const FPrimaryAssetType	USSAssetManager::WeaponItemType = TEXT("Weapon");


USSAssetManager& USSAssetManager::Get()
{
	USSAssetManager* This = Cast<USSAssetManager>(GEngine->AssetManager);

	if (This)
	{
		return *This;
	}
	else
	{
		UE_LOG(LogSS, Fatal, TEXT("Invalid AssetManager in DefaultEngine.ini, must be RPGAssetManager!"));
		return *NewObject<USSAssetManager>(); // never calls this
	}
}

void USSAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	UAbilitySystemGlobals::Get().InitGlobalData();
}


USSItem* USSAssetManager::ForceLoadItem(const FPrimaryAssetId& PrimaryAssetId, bool bLogWarning)
{
	FSoftObjectPath ItemPath = GetPrimaryAssetPath(PrimaryAssetId);

	// This does a synchronous load and may hitch
	USSItem* LoadedItem = Cast<USSItem>(ItemPath.TryLoad());

	if (bLogWarning && LoadedItem == nullptr)
	{
		UE_LOG(LogSS, Warning, TEXT("Failed to load item for identifier %s!"), *PrimaryAssetId.ToString());
	}

	return LoadedItem;
}