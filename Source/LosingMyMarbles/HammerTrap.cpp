// Fill out your copyright notice in the Description page of Project Settings.


#include "HammerTrap.h"

#include "Engine/StaticMeshSocket.h"

// Sets default values
AHammerTrap::AHammerTrap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	Sphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base"));
	Arm = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Arm"));
	Hammer = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hammer"));

	RootComponent = Scene;
	Sphere->SetupAttachment(RootComponent);
	Arm->SetupAttachment(Sphere);
	Hammer->SetupAttachment(Arm);

}

// Called when the game starts or when spawned
void AHammerTrap::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHammerTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

