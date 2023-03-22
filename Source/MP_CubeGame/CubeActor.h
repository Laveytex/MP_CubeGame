// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CubeActor.generated.h"

UCLASS()
class MP_CUBEGAME_API ACubeActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACubeActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	class UStaticMeshComponent* CubeMesh = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	class UTextRenderComponent* CubeValueText = nullptr;
	
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Components")
	class UBoxComponent* BoxCollision;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Components")
	float CubeValue = 2;
	FString TheFloatStr;

	
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Components")
	float Speed = 1200.0f;
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Components")
	bool bStartMove = false;
	//bool SetStartMove();
	void MoveForward(float DeltaTime);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void CubeCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
