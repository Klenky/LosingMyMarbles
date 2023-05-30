// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "MarbleCharacterClass.generated.h"


namespace constants {
	constexpr float CONST_STATIC_MESH_DIAMETER = 100.0f;
}

UCLASS()
class LOSINGMYMARBLES_API AMarbleCharacterClass : public ACharacter
{
	GENERATED_BODY()


public:
	// Sets default values for this pawn's properties
	AMarbleCharacterClass();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* staticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USpringArmComponent* springArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCameraComponent* camera;



	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isMagnetized;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float raytraceLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float gravityForce;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float jumpForce;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float forwardAxisValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float rightAxisValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector lastSavedInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool capsuleRotationSet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FRotator controllerRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector upDownAngle;

	

private:







protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
		void MoveForward(float Axis);

	UFUNCTION()
		float GetMoveForward();

	UFUNCTION()
		void MoveRight(float Axis);

	UFUNCTION()
		float GetMoveRight();

	UFUNCTION()
		void Gravity();

	UFUNCTION(BlueprintCallable)
		void GetMarbleStaticMeshToSpin();

	UFUNCTION()
		void CameraSystem();

	UFUNCTION()
		void RotateCamera();


};


