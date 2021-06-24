// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"
#include "TankPawn.h"
#include "DrawDebugHelpers.h"

ATankPlayerController::ATankPlayerController()
{
	bShowMouseCursor = true;
}

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	TankPawn = Cast<ATankPawn>(GetPawn());
}

void ATankPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveForward", this, &ATankPlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ATankPlayerController::MoveRight);
	InputComponent->BindAxis("RotateRight", this, &ATankPlayerController::RotateRight);
	InputComponent->BindAction("Fire",IE_Pressed, this, &ATankPlayerController::Fire);
	InputComponent->BindAction("AltFire",IE_Pressed, this, &ATankPlayerController::AltFire);
}

void ATankPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FVector MousePosition, MouseDirection;

	DeprojectMousePositionToWorld(MousePosition, MouseDirection);
	
	if (TankPawn)
	{
		FVector TankPosition = TankPawn->GetActorLocation();
		CachedMousePos = MousePosition - TankPosition;
		CachedMousePos.Z = 0;
		CachedMousePos.Normalize();
		CachedMousePos = TankPosition + CachedMousePos * 1000.f;

		DrawDebugLine(GetWorld(), TankPosition, CachedMousePos, FColor::Green, false, 0.f, 0, 5.f);
	}
		
}

FVector ATankPlayerController::GetMousePos() const
{
	return CachedMousePos;
}

void ATankPlayerController::MoveForward(float AxisValue)
{
	if (TankPawn)
	{
		TankPawn->MoveForward(AxisValue);
	}
}

void ATankPlayerController::MoveRight(float AxisValue)
{
	if (TankPawn)
	{
		TankPawn->MoveRight(AxisValue);
	}
}

void ATankPlayerController::RotateRight(float AxisValue)
{
	if (TankPawn)
	{
		TankPawn->RotateRight(AxisValue);
	}
}

void ATankPlayerController::Fire()
{
	if (TankPawn)
	{
		TankPawn->Fire();
	}
}void ATankPlayerController::AltFire()
{
	if (TankPawn)
	{
		TankPawn->AltFire();
	}
}