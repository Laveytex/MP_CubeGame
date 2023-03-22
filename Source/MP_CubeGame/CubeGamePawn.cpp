// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeGamePawn.h"

#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "CubeActor.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ACubeGamePawn::ACubeGamePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;	
	
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	BoxCollision->SetupAttachment(RootComponent);

	CubePointArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	CubePointArrow->SetRelativeLocation(FVector(0,0,100));
	CubePointArrow->SetRelativeRotation(FRotator(-90, 0,0));
	//CubePointArrow->ArrowLength(50.0f);
	CubePointArrow->SetupAttachment(BoxCollision);

	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube Mesh"));
	CubeMesh->SetupAttachment(BoxCollision);
	CubeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = false;
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void ACubeGamePawn::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnCube();
}

// Called every frame
void ACubeGamePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	SetPawnLocation(DeltaTime);
	SpawnCubeTick(DeltaTime);
}

void ACubeGamePawn::MoveRight(float AxisValue)
{
	targetRightAxisValue = AxisValue;
}

void ACubeGamePawn::Fire()
{
	if (CubeActor)
	{
		FAttachmentTransformRules Rule(EAttachmentRule::SnapToTarget, false);
		CubeActor->DetachFromActor(FDetachmentTransformRules( Rule, false));
		CubeActor->bStartMove = true;
		/*FVector Force = ImpulseValue * GetActorForwardVector();
		CubeActor->BoxCollision->SetSimulatePhysics(true);
		CubeActor->BoxCollision->AddImpulse(Force, NAME_None, true);*/
		CubeActor =  nullptr;
		bIsSpawn = false;
	}
}

void ACubeGamePawn::SpawnCubeTick(float DeltaTime)
{
	
	if(CubeActor == nullptr)
	{
		if (!bIsSpawn)
		{
			if (SpawnTime < 0.0f)
			{
				SpawnCube();
				bIsSpawn = true;
				SpawnTime = SpawnTimer;
			}
			else
				SpawnTime -= DeltaTime;
		}
			
	}
}

void ACubeGamePawn::SetPawnLocation(float DeltaTime)
{
	FVector currentLocation = BoxCollision->GetComponentLocation();
	FVector RightVector = BoxCollision->GetRightVector();
	FVector movePosition = currentLocation + RightVector * MoveSpeed * targetRightAxisValue * DeltaTime;
	
	BoxCollision->SetRelativeLocation(movePosition, true);
}

// Called to bind functionality to input
void ACubeGamePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACubeGamePawn::SpawnCube()
{
	//UE_LOG(LogTemp, Warning, TEXT("Fire"));
	FVector Location = CubePointArrow->GetComponentLocation();
	FRotator Rotation = GetActorRotation();
	ACubeActor* SpawnedActor = GetWorld()->SpawnActor<ACubeActor>(CubeActorToSpawn, Location, Rotation);
	
	FAttachmentTransformRules Rule(EAttachmentRule::SnapToTarget, false);
	SpawnedActor->AttachToActor(this, Rule, NAME_None);
	CubeActor = SpawnedActor;
}

