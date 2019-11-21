// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SSTestActor.generated.h"

UCLASS()
class SS_API ASSTestActor : public AActor
{
	GENERATED_BODY()

	UStaticMeshComponent *StaticMeshComponent;


public:	
	// Sets default values for this actor's properties
	ASSTestActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
