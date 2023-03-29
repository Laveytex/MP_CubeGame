// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "CubeActor.generated.h"

class ACubeGamePawn;

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

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Components")
	TSubclassOf<ACubeActor> SpawnCubeClass;

	UPROPERTY(EditInstanceOnly, BluePrintReadWrite, Category = "Components")
	ACubeGamePawn* CubePawn;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Components")
	class USoundBase* MergeSound = nullptr;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Components")
	class USoundBase* HitSound = nullptr;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Components")
	class UNiagaraSystem* CubeExplodeVFX = nullptr;
	
	
	FString CubeValStr;
	
	TArray<ACubeActor*> CubesFieldArray;
	
	void SetText();
	
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Components")
	float Speed = 1200.0f;
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Components")
	bool bStartMove = false;
	//bool SetStartMove();
	void MoveForward(float DeltaTime);

	void PlayHitSound();
	float PlayHitSoundTimer = 1.0f;
	float PlayHitSoundTime = PlayHitSoundTimer;
	bool isCanPlay = true;
	void PlayHitSoundTick(float DeltaTime);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void LaunchToSeem(int8 valToArray, ACubeActor* CubeToLaonch);

	UFUNCTION()
	void CubeCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
