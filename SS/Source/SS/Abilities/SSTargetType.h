// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "SSTargetType.generated.h"


class ASSPlayerCharacter;


/**
 * 
 */
UCLASS()
class SS_API USSTargetType : public UObject
{
	GENERATED_BODY()
	

public:
	// Constructor and overrides
	USSTargetType() {}

	/** Called to determine targets to apply gameplay effects to */
	UFUNCTION(BlueprintNativeEvent)
	void GetTargets(ASSPlayerCharacter* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const;

};


/** Trivial target type that uses the owner */
UCLASS(NotBlueprintable)
class SS_API USSTargetType_UseOwner : public USSTargetType
{
	GENERATED_BODY()

public:
	// Constructor and overrides
	USSTargetType_UseOwner() {}

	/** Uses the passed in event data */
	virtual void GetTargets_Implementation(ASSPlayerCharacter* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const override;
};

/** Trivial target type that pulls the target out of the event data */
UCLASS(NotBlueprintable)
class SS_API USSTargetType_UseEventData : public USSTargetType
{
	GENERATED_BODY()

public:
	// Constructor and overrides
	USSTargetType_UseEventData() {}

	/** Uses the passed in event data */
	virtual void GetTargets_Implementation(ASSPlayerCharacter* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const override;
};
