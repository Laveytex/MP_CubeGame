// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeActor.h"

#include "StaticMeshAttributes.h"
#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"

// Sets default values
ACubeActor::ACubeActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
	RootComponent = BoxCollision;
	BoxCollision->SetSimulatePhysics(true);
	//BoxCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ACubeActor::CubeCollisionSphereBeginOverlap);

	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube Mesh"));
	CubeMesh->SetupAttachment(BoxCollision);
	CubeMesh->SetSimulatePhysics(false);
	
	CubeValueText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Value text"));
	CubeValueText->SetupAttachment(CubeMesh);
	CubeValueText->SetTextRenderColor(FColor::White);
	CubeValueText->SetHorizontalAlignment(EHTA_Center);
	
}

/*
bool ACubeActor::SetStartMove()
{
	
}
*/

void ACubeActor::MoveForward(float DeltaTime)
{
	if (bStartMove)
	{
		FVector XMoveVector(1,0,0);
		FVector NewLocation = GetActorLocation()+XMoveVector*Speed*DeltaTime;
		SetActorLocation(NewLocation, true);
	}
}

// Called when the game starts or when spawned
void ACubeActor::BeginPlay()
{
	Super::BeginPlay();
	
	TheFloatStr = FString::FromInt(CubeValue);
	FText TextVariable = FText::AsCultureInvariant(TheFloatStr);
	CubeValueText->SetText(TextVariable);

	
}

// Called every frame
void ACubeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveForward(DeltaTime);
}

void ACubeActor::CubeCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("ACubeActor::CubeCollisionSphereBeginOverlap - BeginOverlap"));
	bStartMove = false;
	BoxCollision->SetSimulatePhysics(true);
}

