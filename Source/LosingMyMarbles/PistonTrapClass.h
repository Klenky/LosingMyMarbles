// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/BoxComponent.h"
#include "CoreMinimal.h"

#include "PistonTrapClass.generated.h"


UCLASS()
class LOSINGMYMARBLES_API APistonTrapClass : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APistonTrapClass();

	UPROPERTY(EditAnywhere, BlueprintreadWrite)
		UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* StaticMesh1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* InnerPistonBlocking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* SphereCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector StartLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector EndLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float TrapForce = 2000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool TrigLerp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsTriggered;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ZPos;

	


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnOverlapBegins(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void Reset();

	UFUNCTION()
		void FirePiston();

	UFUNCTION()
		void ResetLerp();
};	
