// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "MarbleController.generated.h" 

namespace constants {
	constexpr float CONST_STATIC_MESH_DIAMETER = 100.0f;
	constexpr float CONST_FLOAT_NEGITVE_ONE = -1.0f;
}

UCLASS()
class LOSINGMYMARBLES_API AMarbleController : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMarbleController();


	// ---BLUEPRINT COMPONENTS---
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UFloatingPawnMovement* FPM;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCapsuleComponent* capsuleComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* staticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USpringArmComponent* springArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCameraComponent* camera;

	// ---HEALTH---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Health")
		int maxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Health")
		int currentHealth;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Health")


	
	// ---VARIABLES---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mechanic State")
		bool isMagnetized;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gravity")
		float traceLineLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gravity")
		float gravityForce;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float jumpForce;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float movementSpeed = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Do Not Touch")
		FVector lastSavedInput;





	// bool used for behavioral setting of the controller rotation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Do Not Touch")
		bool capsuleRotationSet;

	
	
	// FRotator used for player direction controlls
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Do Not Touch")
		FRotator controllerRotation;
	// FRotator used for knowing the ground rotation under the marble
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Do Not Touch")
		FRotator groundRotation;

	
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isTraceLineHitting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FRotator groundCurvature;

	UPROPERTY()
		bool isInAir;

private:
	
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	
	
	
	// ---PLAYER INPUT FUNCTIONS---
	UFUNCTION()
		void Jump();

	UFUNCTION()
		void MoveForward(float Axis);

	UFUNCTION()
		float GetMoveForward();

	UFUNCTION()
		void MoveRight(float Axis);

	UFUNCTION()
		float GetMoveRight();


	
	
	
	
	// ---MARBLE PHYSICS FUNCTIONS---
	UFUNCTION()
		void RotateMarbleMesh();

	UFUNCTION()
		void Gravity();

	UFUNCTION()
		float GetFPMVelocityYawDirection();

	
	
	
	
	// ---CAMERA MOVEMENT RELATED FUNCTIONS---
	// function to be called in tick that has all function calls
	UFUNCTION()
		void CameraSystem();
	
	// handle camera rotations
	UFUNCTION()
		void RotateCamera();




	// ---GROUND RELATED ROTATION---
	//This function goes in Tick() and does the traceline while using the set functions to store results.
	UFUNCTION()
		void GroundCurveTrace();
	


	// ---CONTROLLER RELATED ROTATION---
	//controller's yaw is based off of player input and rotates at a simaler rate of the camera
	UFUNCTION()
		void RotateControllerYaw();

	UFUNCTION()
		void RotateControllerPitchRoll();

	// ---RESETTING MARBLE USING CHECKPOINTS---
	UFUNCTION()
		void ResetMarble();

	// ---HEALTH RELATED FUNCTIONS---
	UFUNCTION(BlueprintCallable)
		void SubtractHealth(int value);

	UFUNCTION(BlueprintCallable)
		void AddHealth(int value);

	UFUNCTION(BlueprintCallable)
		void ResetMarbleDeath();
};
