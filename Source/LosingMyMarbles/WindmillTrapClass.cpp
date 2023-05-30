// Fill out your copyright notice in the Description page of Project Settings.


#include "WindmillTrapClass.h"

// Sets default values
AWindmillTrapClass::AWindmillTrapClass()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshRoot  = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Center"));
	StaticMesh1		= CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Blade1"));
	StaticMesh2		= CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Blade2"));
	StaticMesh3		= CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Blade3"));
	StaticMesh4		= CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Blade4"));
	Scene			= CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	RootComponent = Scene;
	StaticMeshRoot->SetupAttachment(Scene);
	StaticMesh1->SetupAttachment(StaticMeshRoot);
	StaticMesh2->SetupAttachment(StaticMeshRoot);
	StaticMesh3->SetupAttachment(StaticMeshRoot);
	StaticMesh4->SetupAttachment(StaticMeshRoot);


	

}

// Called when the game starts or when spawned
void AWindmillTrapClass::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void AWindmillTrapClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Scene->AddWorldRotation(FRotator(0, 0, Spin), false);
}
