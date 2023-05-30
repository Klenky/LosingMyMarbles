// Fill out your copyright notice in the Description page of Project Settings.


#include "PistonTrapClass.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include <string>

// Sets default values
APistonTrapClass::APistonTrapClass()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	StaticMesh			= CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Outer Piston"));
	StaticMesh1			= CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Inner Piston"));
	SphereCollision		= CreateDefaultSubobject<UBoxComponent>(TEXT("Sphere Trigger"));
	InnerPistonBlocking = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InnerPistonBlocking"));

	RootComponent = StaticMesh;
	StaticMesh1->SetupAttachment(StaticMesh);
	InnerPistonBlocking->SetupAttachment(StaticMesh1);
	SphereCollision->SetupAttachment(StaticMesh);

	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &APistonTrapClass::OnOverlapBegins);
	SphereCollision->OnComponentEndOverlap.AddDynamic(this, &APistonTrapClass::OnOverlapEnd);
}

// Called when the game starts or when spawned
void APistonTrapClass::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = StaticMesh1->GetRelativeLocation();
	EndLocation = FVector(StartLocation.X, StartLocation.Y, 90);
}

// Called every frame
void APistonTrapClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TrigLerp == true)
	{
		FTimerHandle FireHandle;
		GetWorldTimerManager().SetTimer(FireHandle, this, &APistonTrapClass::FirePiston, 0.2, false);
		
	}
	else
	{
		StaticMesh1->SetRelativeLocation(FVector(StartLocation.X, StartLocation.Y, FMath::FInterpTo(StaticMesh1->GetRelativeLocation().Z, StartLocation.Z, DeltaTime, 1)));
	}
	
}

void APistonTrapClass::OnOverlapBegins(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Overlap Begin"));
		if (IsTriggered == false)
		{
			//OtherActor->AddActorLocalOffset(FVector(0, 0, 50));
			OtherComp->AddImpulse(FVector(0, 0, TrapForce), NAME_None, true);
			IsTriggered = true;
			TrigLerp = true;
			FTimerHandle UnusedHandle;
			FTimerHandle LerpReset;
			GetWorldTimerManager().SetTimer(LerpReset, this, &APistonTrapClass::ResetLerp, 3, false);
			GetWorldTimerManager().SetTimer(UnusedHandle, this, &APistonTrapClass::Reset, 4, false);
		}
	}
}

void APistonTrapClass::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Overlap End"));
	}
}

void APistonTrapClass::Reset()
{
	IsTriggered = false;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("IsTriggered"));
}

void APistonTrapClass::ResetLerp()
{
	TrigLerp = false;
}


void APistonTrapClass::FirePiston()
{
	StaticMesh1->SetRelativeLocation(FMath::Lerp(StartLocation, EndLocation, 1.0));
}




