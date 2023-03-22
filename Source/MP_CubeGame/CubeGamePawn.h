// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CubeActor.h"
#include "CubeActor.h"
#include "GameFramework/Pawn.h"
#include "CubeGamePawn.generated.h"

UCLASS()
class MP_CUBEGAME_API ACubeGamePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACubeGamePawn();

	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	class UStaticMeshComponent* CubeMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	class UArrowComponent* CubePointArrow;
	
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Components")
	class UBoxComponent* BoxCollision;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Components")
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Components")
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Components")
	TSubclassOf<ACubeActor> CubeActorToSpawn;
	ACubeActor* CubeActor = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
	float MoveSpeed = 300.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
	int ImpulseValue = 500;
	
	float targetRightAxisValue = 0.0f;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Components")
	int CubeValue = 2;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void MoveRight(float AxisValue);
	UFUNCTION()
	void Fire();

	float SpawnTimer = 2.0f;
	float SpawnTime = SpawnTimer;
	
	void SpawnCubeTick(float DeltaTime);
	bool bIsSpawn = true;
		
	void SetPawnLocation(float DeltaTime);
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SpawnCube();

};
