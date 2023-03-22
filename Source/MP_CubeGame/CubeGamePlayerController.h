// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CubeGamePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MP_CUBEGAME_API ACubeGamePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY()
	class ACubeGamePawn* CubePawn;

	virtual void BeginPlay() override;
	
	
	void MoveRight(float AxisValue);
	void Fire();

	virtual void SetupInputComponent() override;
};
