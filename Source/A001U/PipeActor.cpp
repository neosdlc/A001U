// Fill out your copyright notice in the Description page of Project Settings.


#include "PipeActor.h"

// Sets default values
APipeActor::APipeActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene Component"));
    RootComponent = DefaultSceneRootComponent;

    MainSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("MainScene Component"));
    MainSceneComponent->SetupAttachment(DefaultSceneRootComponent);

    PipeMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PipeMesh Component"));
    PipeMeshComponent->SetupAttachment(MainSceneComponent);

    CapPipeMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CapPipeMesh Component"));
    CapPipeMeshComponent->SetupAttachment(PipeMeshComponent);

}

// Called when the game starts or when spawned
void APipeActor::BeginPlay()
{
	Super::BeginPlay();

	FVector CapPipeMeshInitialLocation = FVector::ZeroVector;
    CapPipeMeshInitialLocation.Z = 4.0f;
    CapPipeMeshComponent->SetRelativeLocation(CapPipeMeshInitialLocation);
	
}

// Called every frame
void APipeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MovePipe(DeltaTime);

}

void APipeActor::MovePipe(float DeltaTime)
{
    FVector NewLocation = FVector::ZeroVector;
    NewLocation.Y = -150.0f;

    FVector PipeNewLocation = NewLocation * DeltaTime;

    AddActorLocalOffset(PipeNewLocation);
}
