// Fill out your copyright notice in the Description page of Project Settings.


#include "SSAbilitySystemComponent.h"
#include "SSGameplayAbility.h"
#include "AbilitySystemGlobals.h"


USSAbilitySystemComponent::USSAbilitySystemComponent() {}

void USSAbilitySystemComponent::GetActiveAbilitiesWithTags(const FGameplayTagContainer & GameplayTagContainer, TArray<class USSGameplayAbility*>& ActiveAbilities)
{
	TArray<FGameplayAbilitySpec*> AbilitiesToActivate;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(GameplayTagContainer, AbilitiesToActivate, false);

	// Iterate the list of all ability specs
	for (FGameplayAbilitySpec* Spec : AbilitiesToActivate)
	{
		// Iterate all instances on this ability spec
		TArray<UGameplayAbility*> AbilityInstances = Spec->GetAbilityInstances();

		for (UGameplayAbility* ActiveAbility : AbilityInstances)
		{
			ActiveAbilities.Add(Cast<USSGameplayAbility>(ActiveAbility));
		}
	}
}

int32 USSAbilitySystemComponent::GetDefaultAbilityLevel() const
{
	return int32();
}

USSAbilitySystemComponent * USSAbilitySystemComponent::GetAbilitySystemComponentFromActor(const AActor * Actor, bool LookForComponent)
{
	return Cast<USSAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor, LookForComponent));
}
