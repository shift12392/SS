// Fill out your copyright notice in the Description page of Project Settings.


#include "SSGameplayAbility.h"
#include "SSCharacter.h"
#include "SSAbilitySystemComponent.h"
#include "SSTargetType.h"

USSGameplayAbility::USSGameplayAbility() {}

FSSGameplayEffectContainerSpec USSGameplayAbility::MakeEffectContainerSpecFromContainer(const FSSGameplayEffectContainer & Container, const FGameplayEventData & EventData, int32 OverrideGameplayLevel)
{
	// First figure out our actor info
	FSSGameplayEffectContainerSpec ReturnSpec;
	AActor* OwningActor = GetOwningActorFromActorInfo();
	ASSCharacter* OwningCharacter = Cast<ASSCharacter>(OwningActor);
	USSAbilitySystemComponent* OwningASC = USSAbilitySystemComponent::GetAbilitySystemComponentFromActor(OwningActor);

	if (OwningASC)
	{
		// If we have a target type, run the targeting logic. This is optional, targets can be added later
		if (Container.TargetType.Get())
		{
			TArray<FHitResult> HitResults;
			TArray<AActor*> TargetActors;
			const USSTargetType* TargetTypeCDO = Container.TargetType.GetDefaultObject();
			AActor* AvatarActor = GetAvatarActorFromActorInfo();
			TargetTypeCDO->GetTargets(OwningCharacter, AvatarActor, EventData, HitResults, TargetActors);
			ReturnSpec.AddTargets(HitResults, TargetActors);
		}

		// If we don't have an override level, use the default on the ability itself
		if (OverrideGameplayLevel == INDEX_NONE)
		{
			OverrideGameplayLevel = OverrideGameplayLevel = this->GetAbilityLevel(); //OwningASC->GetDefaultAbilityLevel();
		}

		// Build GameplayEffectSpecs for each applied effect
		for (const TSubclassOf<UGameplayEffect>& EffectClass : Container.TargetGameplayEffectClasses)
		{
			ReturnSpec.TargetGameplayEffectSpecs.Add(MakeOutgoingGameplayEffectSpec(EffectClass, OverrideGameplayLevel));
		}
	}
	return ReturnSpec;
}

FSSGameplayEffectContainerSpec USSGameplayAbility::MakeEffectContainerSpec(FGameplayTag ContainerTag, const FGameplayEventData & EventData, int32 OverrideGameplayLevel)
{
	FSSGameplayEffectContainer* FoundContainer = EffectContainerMap.Find(ContainerTag);

	if (FoundContainer)
	{
		return MakeEffectContainerSpecFromContainer(*FoundContainer, EventData, OverrideGameplayLevel);
	}
	return FSSGameplayEffectContainerSpec();
}

TArray<FActiveGameplayEffectHandle> USSGameplayAbility::ApplyEffectContainerSpec(const FSSGameplayEffectContainerSpec & ContainerSpec)
{
	TArray<FActiveGameplayEffectHandle> AllEffects;

	// Iterate list of effect specs and apply them to their target data
	for (const FGameplayEffectSpecHandle& SpecHandle : ContainerSpec.TargetGameplayEffectSpecs)
	{
		AllEffects.Append(K2_ApplyGameplayEffectSpecToTarget(SpecHandle, ContainerSpec.TargetData));
	}
	return AllEffects;
}

TArray<FActiveGameplayEffectHandle> USSGameplayAbility::ApplyEffectContainer(FGameplayTag ContainerTag, const FGameplayEventData & EventData, int32 OverrideGameplayLevel)
{
	FSSGameplayEffectContainerSpec Spec = MakeEffectContainerSpec(ContainerTag, EventData, OverrideGameplayLevel);

	return ApplyEffectContainerSpec(Spec);
}
