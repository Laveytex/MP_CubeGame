// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeGamePlayerController.h"

#include "CubeGamePawn.h"

void ACubeGamePlayerController::BeginPlay()
{
	Super::BeginPlay();

	CubePawn = Cast<ACubeGamePawn>(GetPawn());
}

void ACubeGamePlayerController::MoveRight(float AxisValue)
{
	if (CubePawn)
		CubePawn->MoveRight(AxisValue);
}

void ACubeGamePlayerController::Fire()
{
	CubePawn->Fire();
}

void ACubeGamePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis("MoveRight", this, &ACubeGamePlayerController::MoveRight);
	InputComponent->BindAction("Fire",IE_Pressed, this, &ACubeGamePlayerController::Fire);
}
