// Fill out your copyright notice in the Description page of Project Settings.


#include "MarbleController.h"
#include "Math/RotationMatrix.h"
#include "Math/UnrealMathUtility.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include <string>
#include <Kismet/KismetMathLibrary.h>

// Sets default values
AMarbleController::AMarbleController()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// creation of object componets
	capsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("capsule");
	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>("marble mesh");
	springArm = CreateDefaultSubobject<USpringArmComponent>("spring arm");
	camera = CreateDefaultSubobject<UCameraComponent>("Player Camera");
	FPM = CreateDefaultSubobject<UFloatingPawnMovement>("FPM");
	


	// setup heierachy
	RootComponent = capsuleComponent;
	staticMesh->SetupAttachment(RootComponent);
	springArm->SetupAttachment(RootComponent);
	camera->SetupAttachment(springArm);


	// initialize variables
	capsuleRotationSet = false;
	lastSavedInput = FVector(0, 0, 0);
	controllerRotation = FRotator(0, 0, 0);


}

// Called when the game starts or when spawned
void AMarbleController::BeginPlay()
{
	Super::BeginPlay();
	currentHealth = maxHealth;
}

// Called every frame
void AMarbleController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	GEngine->AddOnScreenDebugMessage(1, 0.01f, FColor::Blue, FPM->Velocity.ToString());
	
	RotateMarbleMesh();
	Gravity();
	GroundCurveTrace();
	RotateControllerPitchRoll();
	CameraSystem();
	
	

}

// Called to bind functionality to input
void AMarbleController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &AMarbleController::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMarbleController::MoveRight);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMarbleController::Jump);
}

void AMarbleController::Jump()
{
	if (!isInAir) {
		FPM->UpdatedPrimitive->AddImpulse( (UKismetMathLibrary::GetUpVector(controllerRotation) * jumpForce), NAME_None, true);
	}
}




void AMarbleController::MoveForward(float Axis)
{
	FPM->AddInputVector( (UKismetMathLibrary::GetForwardVector(controllerRotation) * (Axis * movementSpeed)), true);	
}

float AMarbleController::GetMoveForward()
{
	return this->GetInputAxisValue("MoveForward");
}




void AMarbleController::MoveRight(float Axis)
{
	
		if (GetMoveForward() != -1)
		{
			FPM->AddInputVector( (UKismetMathLibrary::GetRightVector(controllerRotation) * (Axis * movementSpeed)), true);
			
		}
		else
		{
			FPM->AddInputVector( (UKismetMathLibrary::GetRightVector(controllerRotation) * (Axis * movementSpeed) * -1.0f), true);
		}
	
}

float AMarbleController::GetMoveRight()
{
	return this->GetInputAxisValue("MoveRight");
}





void AMarbleController::Gravity()
{
	FPM->UpdatedPrimitive->AddForce((UKismetMathLibrary::GetUpVector(controllerRotation) * (gravityForce * -1.0f)), NAME_None, true);
}




float AMarbleController::GetFPMVelocityYawDirection()
{
	return FMath::RadiansToDegrees(FMath::Atan2(lastSavedInput.Y, lastSavedInput.X));
}




void AMarbleController::RotateMarbleMesh()
{
	//TODO: only works for 2D rotation, does not rotates with gravity so spinning looks off when not using original upright gravity, figure it out.
	
	FRotator marbleDirection = FRotator(
		FPM->Velocity.X * -1.0f / (constants::CONST_STATIC_MESH_DIAMETER),
		0,
		FPM->Velocity.Y * 1.0f / (constants::CONST_STATIC_MESH_DIAMETER)
	);

	staticMesh->AddWorldRotation(marbleDirection, false);
}

void AMarbleController::CameraSystem()
{
	if (GetMoveForward() != 0.0f || GetMoveRight() != 0.0f)
	{
		lastSavedInput = FPM->Velocity;
	}
	
	// rotate the controller FRotator
	RotateControllerYaw();
	// rotate the camera
	RotateCamera();
}




void AMarbleController::RotateCamera() 
{
	//The lerped FQuat to update the rotation of the springArm smoothly.
	FQuat lerpedRotation = FQuat::Slerp(
		springArm->GetRelativeRotation().Quaternion(),
		FRotator(controllerRotation.Pitch, GetFPMVelocityYawDirection(), controllerRotation.Roll).Quaternion(),
		0.032f
	);
	
	// setting the springArm's new rotation to the lerped rotation.
	springArm->SetRelativeRotation(lerpedRotation);
}



void AMarbleController::RotateControllerYaw()
{
	if (GetMoveForward() == 0.0f && GetMoveRight() == 0.0f)
	{
		if (capsuleRotationSet == false)
		{
			// set the controller's yaw to the velocity direction of the marble.
			//Previous yaw = GetFPMVelocityYawDirection()
			controllerRotation = FRotator(controllerRotation.Pitch, groundCurvature.Yaw, controllerRotation.Roll);
			
			capsuleRotationSet = true;
		}
	}
	else
	{
		capsuleRotationSet = false;

		if (GetMoveForward() != -1.0f)
		{
			// getting only the lerped yaw value to uupdate the controller's yaw
			float lerpedYaw = FQuat::Slerp(
				FRotator(0.0f, controllerRotation.Yaw, 0.0f).Quaternion(),
				FRotator(0.0f, groundCurvature.Yaw, 0.0f).Quaternion(),
				0.032f
			).Rotator().Yaw;

			controllerRotation = FRotator(controllerRotation.Pitch, lerpedYaw, controllerRotation.Roll);
		}
	}

}

void AMarbleController::RotateControllerPitchRoll()
{
	
	// if trace is hitting is essentially whether the marble is in the air or not.
	if (isTraceLineHitting)
	{
		// update controller's Pitch and Roll with groundRotation's 
		controllerRotation = FRotator(groundRotation.Pitch, controllerRotation.Yaw, groundRotation.Roll);
		// testing
		//controllerRotation = groundCurvature;
	}
	else
	{
		// if marble is in the air it forces pitch to level at zero while still updating the Roll
		controllerRotation = FRotator(0.0f, controllerRotation.Yaw, groundRotation.Roll);
		// testing
		//controllerRotation = groundCurvature;
	}
}




void AMarbleController::GroundCurveTrace()
{
	FHitResult hitResult = FHitResult();
	FVector startTrace = GetActorLocation();
	FVector endTrace = startTrace + ( UKismetMathLibrary::GetUpVector(groundRotation) * (traceLineLength * -1.0f) );
	
	// line trace the ground and set ground curvature to the outHit normal of the ground if true
	if (GetWorld()->LineTraceSingleByChannel(hitResult, startTrace, endTrace, ECC_Visibility, FCollisionQueryParams()))
	{
		isTraceLineHitting = true;
		
		groundCurvature = FRotationMatrix::MakeFromXZ(
			FVector::VectorPlaneProject(lastSavedInput, hitResult.Normal),
			hitResult.Normal
		).Rotator();
		
		// update groundRotation with a lerp so that the camera translates smoothly too.
		groundRotation = FQuat::Slerp(groundRotation.Quaternion(), groundCurvature.Quaternion(), 0.8f).Rotator();
	} 
	else
	{
		isTraceLineHitting = false;
		
		groundCurvature = FRotationMatrix::MakeFromXZ(
			FVector::VectorPlaneProject(lastSavedInput, hitResult.Normal),
			hitResult.Normal
		).Rotator();

		// update groundRotation with a lerp so that the camera translates smoothly too.
		groundRotation = FQuat::Slerp(groundRotation.Quaternion(), groundCurvature.Quaternion(), 0.8f).Rotator();
		
	}
	
	if (hitResult.Distance > 45.0f && hitResult.Distance < 55.0f) {
		isInAir = false;
	}
	else {
		isInAir = true;
	}
	
}


// ---RESET MARBLE---
void AMarbleController::ResetMarble()
{
	FPM->Velocity = FVector(0, 0, 0);
	FHitResult hitResult;
	FQuat newRotation = FQuat(0.0, 0.0, 0.0, 0.0);
	FVector startPos = FVector(-24639.000008, -12716.323005, 1700);
	//this->SetActorLocationAndRotation(startPos, newRotation, false, 0, ETeleportType::ResetPhysics);
	FPM->Velocity = FVector(0, 0, 0);
	this->SetActorLocation(startPos, true, 0, ETeleportType::ResetPhysics);
	FPM->Velocity = FVector(0, 0, 0);
	//this->SetActorTransform
}



// ---HEALTH RELATED FUNCTIONS---
void AMarbleController::SubtractHealth(int value)
{
	currentHealth -= value;

	if (currentHealth <= 0) {
		//ResetMarbleDeath();
		FPM->Velocity = FVector(0, 0, 0);
	}

	//TODO: update marble crack pattern here.
}

void AMarbleController::AddHealth(int value)
{
	
	
	if ( (currentHealth + value) > maxHealth) {
		currentHealth = maxHealth;
	}
	else {
		currentHealth += value;
	}
	
}

void AMarbleController::ResetMarbleDeath()
{
	currentHealth = maxHealth;
	
	//TODO: have animation lock player input
	// call death anim here.
	//ResetMarble();
	FPM->Velocity = FVector(0, 0, 0);
}





