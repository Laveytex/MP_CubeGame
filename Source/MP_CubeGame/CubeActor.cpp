// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeActor.h"

#include "CubeGamePawn.h"
#include "NiagaraComponent.h"
#include "EngineUtils.h"
#include "NiagaraFunctionLibrary.h"
#include "StaticMeshAttributes.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACubeActor::ACubeActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	

	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube Mesh"));
	RootComponent = CubeMesh;
	CubeMesh->SetGenerateOverlapEvents(false);
	CubeMesh->SetSimulatePhysics(false);
	CubeMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CubeMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CubeMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
	BoxCollision->SetupAttachment(CubeMesh);
	BoxCollision->SetSimulatePhysics(false);
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	BoxCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, 
		&ACubeActor::CubeCollisionSphereBeginOverlap);
		
	
	CubeValueText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Value text"));
	CubeValueText->SetupAttachment(CubeMesh);
	CubeValueText->SetTextRenderColor(FColor::White);
	CubeValueText->SetHorizontalAlignment(EHTA_Center);
	
}

void ACubeActor::SetText()
{
	CubeValStr = FString::FromInt(CubeValue);
	FText TextVariable = FText::AsCultureInvariant(CubeValStr);
	CubeValueText->SetText(TextVariable);
	//CubeValueText->SetTextRenderColor(FColor::Yellow);
}

void ACubeActor::MoveForward(float DeltaTime)
{
	if (bStartMove)
	{
		FVector XMoveVector(1,0,0);
		FVector NewLocation = GetActorLocation()+XMoveVector*Speed*DeltaTime;
		SetActorLocation(NewLocation, true);
	}
}

void ACubeActor::PlayHitSound()
{
	if (isCanPlay)
	{
		if (HitSound)
			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), HitSound, GetActorLocation());
		isCanPlay =false;
	}
		
	
}

void ACubeActor::PlayHitSoundTick(float DeltaTime)
{
	if (PlayHitSoundTime < 0.0f)
	{
		isCanPlay = true;
		PlayHitSoundTime = PlayHitSoundTimer;
	}
	else
		PlayHitSoundTime -= DeltaTime;
}

// Called when the game starts or when spawned
void ACubeActor::BeginPlay()
{
	Super::BeginPlay();
	
	SetText();
	AActor* FoundActor =  UGameplayStatics::GetActorOfClass(GetWorld(), ACubeGamePawn::StaticClass());
	CubePawn = Cast<ACubeGamePawn>(FoundActor);
}

// Called every frame
void ACubeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveForward(DeltaTime);
	PlayHitSoundTick(DeltaTime);
}

void ACubeActor::CubeCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor,	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
	bool bFromSweep, const FHitResult& SweepResult)
{
	PlayHitSound();
	
	CubeMesh->SetSimulatePhysics(true);
	
	if (GetName()!=OtherActor->GetName())
	{
		ACubeActor* GetActor = Cast<ACubeActor>(OtherActor);
		if (GetActor->CubeValue == CubeValue)
		{
			FTransform spawnTransform(OtherActor->GetActorRotation(),
				OtherActor->GetActorLocation(), OtherActor->GetActorScale());
			if (MergeSound)
				UGameplayStatics::SpawnSoundAtLocation(GetWorld(), MergeSound, OtherActor->GetActorLocation());

			
			ACubeActor* Cube = GetWorld()->SpawnActorDeferred<ACubeActor>(SpawnCubeClass,
				spawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
			
			if(Cube)
			{
				
				float valToArray = CubeValue*2;
				Cube->CubeValue = valToArray;
				CubePawn->CreateCubeParams(valToArray);
				int8 arrayValNum = CubePawn->CubeNumArray.Find(valToArray);
				Cube->CubeValueText->SetTextRenderColor(CubePawn->CubeNumColor[arrayValNum]);

				UMaterialInstanceDynamic* CubeMaterial = UMaterialInstanceDynamic::Create(Cube->CubeMesh->GetMaterial(0), Cube);
				CubeMaterial->SetVectorParameterValue(FName(TEXT("GlowColor")), CubePawn->CubeNumColor[arrayValNum]);
				CubeMaterial->SetVectorParameterValue(FName(TEXT("GrooveColor")), CubePawn->CubeNumColor[arrayValNum]);
				Cube->CubeMesh->SetMaterial(0, CubeMaterial);

				Cube->bStartMove = true;
				
				UGameplayStatics::FinishSpawningActor(Cube, spawnTransform);

				
				if (CubeExplodeVFX)
				{
					UNiagaraComponent* ExplodeVFX = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),
						CubeExplodeVFX, spawnTransform.GetLocation());
					//ExplodeVFX->SetVectorParameter(FName (TEXT("Color")), FVector(10,0,10));
					ExplodeVFX->SetNiagaraVariableLinearColor(("Color"),CubePawn->CubeNumColor[arrayValNum]);
				}
				
				
				LaunchToSeem(valToArray, Cube);
				
				OtherActor->Destroy();
				Destroy();
			}
			
			/*UE_LOG(LogTemp, Warning, TEXT("ACubeActor::CubeCollisionSphereBeginOverlap - BeginOverlap Actor: %s"),
			*OtherActor->GetName())*/
		}
		
		
	}
		
	for (TActorIterator<ACubeActor> It(GetWorld(), ACubeActor::StaticClass()); It; ++It)
	{
		ACubeActor* actor = *It;
		if (actor != NULL)
		{
			CubesFieldArray.Add(actor);
		}
	}
}
	

void ACubeActor::LaunchToSeem(int8 valToArray, ACubeActor* CubeToLaonch)
{
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACubeActor::StaticClass(), CubesArray);

	ACubeActor* TargetCube = nullptr;
	
	for (auto i = 0; i < CubesFieldArray.Num(); i++)
	{
		if (CubesFieldArray[i]->CubeValue == valToArray && CubesFieldArray[i]->GetName() != CubeToLaonch->GetName()
			&& CubesFieldArray[i]->bStartMove != false)
		{
			TargetCube = CubesFieldArray[i];
			//return;
		}
		//return;
	}

	if (TargetCube)
	{
		FVector impuls = TargetCube->GetActorLocation() - CubeToLaonch->GetActorLocation() + FVector(0,0,600);
		CubeToLaonch->CubeMesh->AddImpulse(impuls, NAME_None, true);
		UE_LOG(LogTemp, Warning, TEXT("ACubeActor::LaunchToSeem - impuls: %s, TargetCube: %s, Cube: %s"), *impuls.ToString(), *TargetCube->GetName(), *CubeToLaonch->GetName());
	}
	
}

