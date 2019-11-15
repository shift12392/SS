// Fill out your copyright notice in the Description page of Project Settings.


#include "SSTestActor.h"
#include "Math/Quat.h"

// Sets default values
ASSTestActor::ASSTestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

	FVector RVA(1.0f, 2.0f, 3.0f);
	RVA.Normalize();
	FVector RVB(4.0f, 5.0f, 6.0f);
	RVB.Normalize();

	acg::Math::Vector RVC(1.0f, 2.0f, 3.0f);
	RVC.Normalize();
	acg::Math::Vector RVD(4.0f, 5.0f, 6.0f);
	RVD.Normalize();

	float Angle = FMath::DegreesToRadians(60.0f);

	FQuat QA(RVA, Angle);
	FQuat QB(RVB, Angle);
	FQuat QAB = QA * QB;
	acg::Math::Quat QC(RVC, Angle);
	acg::Math::Quat QD(RVD, Angle);
	acg::Math::Quat QCD = QC * QD;


}

// Called when the game starts or when spawned
void ASSTestActor::BeginPlay()
{
	Super::BeginPlay();
	



}

// Called every frame
void ASSTestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

