// Fill out your copyright notice in the Description page of Project Settings.


#include "PipeSpawnerActor.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "TimerManager.h"

// Sets default values
APipeSpawnerActor::APipeSpawnerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APipeSpawnerActor::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorldTimerManager().SetTimer(TimerHandle, this, &APipeSpawnerActor::SpawnPipes, 2.0f, true);

}

// Called every frame
void APipeSpawnerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APipeSpawnerActor::SpawnPipes()
{
    AActor* PipeUp = GetWorld()->SpawnActor<AActor>(PipeActorClass, GetPipeSpawnLocation(EPipeType::Up), GetPipeSpawnRotation(EPipeType::Up));
    AActor* PipeDown = GetWorld()->SpawnActor<AActor>(PipeActorClass, GetPipeSpawnLocation(EPipeType::Down), GetPipeSpawnRotation(EPipeType::Down));

    SetPipeScale3DTransformations(PipeUp);
    SetPipeScale3DTransformations(PipeDown);
}

void APipeSpawnerActor::SetPipeScale3DTransformations(AActor* PipeActor)
{
    if (PipeActor)
    {
        UStaticMeshComponent* PipeStaticMeshComponent = PipeActor->GetComponentByClass<UStaticMeshComponent>();
        PipeStaticMeshComponent->SetWorldScale3D(FVector(1.0f, 1.0f, 100.0f));

        UStaticMeshComponent* CapPipeStaticMeshComponent = Cast<UStaticMeshComponent>(PipeStaticMeshComponent->GetChildComponent(0));
        CapPipeStaticMeshComponent->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
    }
}

FVector APipeSpawnerActor::GetPipeSpawnLocation(EPipeType PipeType)
{
    UWorld* World = GetWorld();

    if (World == nullptr)
    {
        return FVector::ZeroVector;
    }

    APlayerController* PlayerController = World->GetFirstPlayerController();

    if (PlayerController == nullptr)
    {
        return FVector::ZeroVector;
    }

    USpringArmComponent* PawnSpringArmComponent = PlayerController->GetPawn()->FindComponentByClass<USpringArmComponent>();

    if (PawnSpringArmComponent == nullptr)
    {
        return FVector::ZeroVector;
    }

    float PawnSpringArmTargetArmLength = PawnSpringArmComponent->TargetArmLength;

    const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());

    FVector WorldLocation;
    FVector WorldDirection;

    if (!PlayerController->DeprojectScreenPositionToWorld(ViewportSize.X, ViewportSize.Y, WorldLocation, WorldDirection))
    {
        return FVector::ZeroVector;
    }

    if (PipeType == EPipeType::Up)
    {
        WorldLocation.X = 0.0f;
        WorldLocation.Y += PawnSpringArmTargetArmLength;
        WorldLocation.Z *= -(PawnSpringArmTargetArmLength / 10.0f);

        return WorldLocation;
    }
    else if (PipeType == EPipeType::Down)
    {
        WorldLocation.X = 0.0f;
        WorldLocation.Y += PawnSpringArmTargetArmLength;
        WorldLocation.Z *= PawnSpringArmTargetArmLength / 10.0f;

        return WorldLocation;
    }

    return FVector::ZeroVector;
}

FRotator APipeSpawnerActor::GetPipeSpawnRotation(EPipeType PipeType)
{
    if (PipeType == EPipeType::Up)
    {
        FRotator PipeUpRotation = FRotator::ZeroRotator;
        PipeUpRotation.Roll = -180.0f;
        PipeUpRotation.Yaw = 180.0f;

        return PipeUpRotation;
    }
    else if (PipeType == EPipeType::Down)
    {
        FRotator PipeDownRotation = FRotator::ZeroRotator;

        return PipeDownRotation;
    }

    return FRotator::ZeroRotator;
}
