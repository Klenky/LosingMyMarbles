// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HammerTrap.generated.h"


UCLASS()
class LOSINGMYMARBLES_API AHammerTrap : public AActor
{
	GENERATED_BODY()

	
	
public:	
	// Sets default values for this actor's properties
	AHammerTrap();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USceneComponent* Scene;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* Sphere;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* Arm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* Hammer;

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



};
