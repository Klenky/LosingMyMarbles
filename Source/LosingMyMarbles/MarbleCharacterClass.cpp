// Fill out your copyright notice in the Description page of Project Settings.

#include "MarbleCharacterClass.h"
#include "Math/RotationMatrix.h"
#include "Math/UnrealMathUtility.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <string>



// Sets default values
AMarbleCharacterClass::AMarbleCharacterClass()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// creation of object componets
	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>("marble mesh");
	springArm = CreateDefaultSubobject<USpringArmComponent>("spring arm");
	camera = CreateDefaultSubobject<UCameraComponent>("Player Camera");


	// setup heierachy
	staticMesh->SetupAttachment(RootComponent);

	springArm->SetupAttachment(RootComponent);
	camera->SetupAttachment(springArm);


	// initialize variables
	capsuleRotationSet = false;
	lastSavedInput = FVector(0, 0, 0);
	controllerRotation = FRotator(0, 0, 0);


}

// Called when the game starts or when spawned
void AMarbleCharacterClass::BeginPlay()
{
	Super::BeginPlay();


}

// Called every frame
void AMarbleCharacterClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	

	GetMarbleStaticMeshToSpin();
	Gravity();
	CameraSystem();





}

// Called to bind functionality to input
void AMarbleCharacterClass::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMarbleCharacterClass::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMarbleCharacterClass::MoveRight);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMarbleCharacterClass::Jump);

}

void AMarbleCharacterClass::MoveForward(float Axis)
{
	if (Axis != 0) {
		AddMovementInput(controllerRotation.Vector(), Axis, true);
		
	}	
	else {
	}
}
float AMarbleCharacterClass::GetMoveForward()
{
	return this->GetInputAxisValue("MoveForward");
}

void AMarbleCharacterClass::MoveRight(float Axis)
{
	
	if (Axis != 0) {
		FRotator rightDir = FRotator(controllerRotation.Pitch, controllerRotation.Yaw + 90.0f, controllerRotation.Roll);
		if (GetMoveForward() != -1)
		{
			AddMovementInput(rightDir.Vector(), Axis, true);
		}
		else
		{
			AddMovementInput(rightDir.Vector()*-1, Axis, true);
		}
	}
	else {

	}
}

float AMarbleCharacterClass::GetMoveRight()
{
	return this->GetInputAxisValue("MoveRight");
}



void AMarbleCharacterClass::Gravity()
{
	/*FVector startForward = rotationBox->GetRelativeLocation();
	FVector endForward = startForward + (FPM->GetLastInputVector() * 1000);

	DrawDebugLine(GetWorld(), startForward, endForward, FColor::Blue, false, 0.1f, 0.0f, 5.0f);
	//new
	FHitResult hitResult;
	FVector startLocation = rotationBox->GetRelativeLocation();
	FVector endLocation = startLocation + rotationBox->GetUpVector() * raytraceLength * -1;
	if (GetWorld()->LineTraceSingleByChannel(hitResult, startLocation, endLocation, ECC_WorldStatic)) {

		DrawDebugLine(GetWorld(), startLocation, endLocation, FColor::Green, false, 0.1f, 0.0f, 5.0f);


		if ( hitResult.GetActor()->ActorHasTag("floor") == true) {
			GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Green, hitResult.GetActor()->GetName());


			rotationBox->SetRelativeRotation(hitResult.GetActor()->GetActorRotation());



			staticMesh->SetPhysicsLinearVelocity((rotationBox->GetUpVector() * -gravityForce), true);
		}
		// TODO: try to get rotationBox to rotate on the y-axis
		// TODO: smooth out the camera set_rotations
	} else {
		rotationBox->SetRelativeRotation(FRotator(0, 0, 0), true);
		//springArm->SetRelativeRotation(rotationBox->GetRelativeRotation() + FRotator(0, -20.0f, 0));
		staticMesh->SetPhysicsLinearVelocity(FVector(0, 0, -gravityForce), true);
	}

	GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Green, (rotationBox->GetUpVector() * -gravityForce).ToCompactString());
	*/
}

void AMarbleCharacterClass::GetMarbleStaticMeshToSpin()
{
	FRotator marbleDirection = FRotator(
		GetCharacterMovement()->Velocity.X * -1.0f / (constants::CONST_STATIC_MESH_DIAMETER),
		0,
		GetCharacterMovement()->Velocity.Y * 1.0f / (constants::CONST_STATIC_MESH_DIAMETER)
	);

	staticMesh->AddWorldRotation(marbleDirection, false);
}

void AMarbleCharacterClass::CameraSystem()
{

	// only store input when movement is actually taking place
	// *lastSavedInput is based off of velocity, used ArcTan(y/x) to get theta for direction later
	if ((GetMoveForward() != 0) || (GetMoveRight() != 0)) {
		lastSavedInput = GetCharacterMovement()->Velocity;
	}

	// rotate the camera
	RotateCamera();

	// Applies controller rotation once there is zero input on the controller and if camera reached
	// range of degrees to cause a reset.
	/*if ((FMath::Abs(springArm->GetComponentRotation().Yaw) > (FMath::Abs(FMath::RadiansToDegrees(FMath::Atan2(lastSavedInput.Y, lastSavedInput.X)))) - 5.0f) && (FMath::Abs(springArm->GetComponentRotation().Yaw) < (FMath::Abs(FMath::RadiansToDegrees(FMath::Atan2(lastSavedInput.Y, lastSavedInput.X)))) + 5.0f)) {*/



	if ((GetMoveForward() == 0.0f) && (GetMoveRight() == 0.0f)) {
		if (!capsuleRotationSet) {
			GEngine->AddOnScreenDebugMessage(1, 10.0f, FColor::Purple, "True");
			controllerRotation = FRotator(0, FMath::RadiansToDegrees(FMath::Atan2(lastSavedInput.Y, lastSavedInput.X)), 0);
			capsuleRotationSet = true;
		}
	}
	else {
		capsuleRotationSet = false;
		if (GetMoveForward() != -1)
		{
			FRotator NewRotation = FRotator(FQuat::Slerp(
				controllerRotation.Quaternion(),
				FRotator(0, FMath::RadiansToDegrees(FMath::Atan2(lastSavedInput.Y, lastSavedInput.X)), 0).Quaternion(),
				0.032f
			));
			controllerRotation = NewRotation;
		}else {
			/*if (GetMoveRight() != 0) {
				FRotator NewRotation = FRotator(FQuat::Slerp(
					FQuat(0, controllerRotation.Yaw * -1, 0, 0),
					FRotator(0, FMath::RadiansToDegrees(FMath::Atan2(lastSavedInput.Y, lastSavedInput.X)),  0).Quaternion(),
					0.032f
				));
				controllerRotation = NewRotation;
			}*/
		}
		
	}

}

void AMarbleCharacterClass::RotateCamera() {

	// shortest path to new yaw direction on the spring arm
	FRotator NewRotation = FRotator(FQuat::Slerp(
		springArm->GetComponentRotation().Quaternion(),
		FRotator(upDownAngle.Rotation().Pitch, FMath::RadiansToDegrees(FMath::Atan2(lastSavedInput.Y, lastSavedInput.X)), upDownAngle.Rotation().Roll).Quaternion(),
		0.03f
	));

	springArm->SetWorldRotation(NewRotation);

	
}






