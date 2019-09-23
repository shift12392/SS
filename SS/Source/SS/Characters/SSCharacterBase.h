// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Abilities/SSAbilitySystemComponent.h"
#include "SSCharacterBase.generated.h"

UCLASS()
class SS_API ASSCharacterBase : public ACharacter , public IAbilitySystemInterface
{
	GENERATED_BODY()

protected:
	/** The level of this character, should not be modified directly once it has already spawned */
	UPROPERTY(EditAnywhere, Replicated, Category = Abilities)
	int32 CharacterLevel;

	/** The component used to handle ability system interactions */
	UPROPERTY()
	USSAbilitySystemComponent* AbilitySystemComponent;

	bool bAbilitiesInitialized;


public:
	// Sets default values for this character's properties
	ASSCharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void UnPossessed() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Returns the ability system component to use for this actor. It may live on another actor, such as a Pawn using the PlayerState's component */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const;

};
