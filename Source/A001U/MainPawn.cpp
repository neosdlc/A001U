// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPawn.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/Engine.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

// Sets default values
AMainPawn::AMainPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot Component"));
    RootComponent = DefaultSceneRootComponent;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm Component"));
    SpringArmComponent->TargetArmLength = SpringArmTargetArmLength;
    SpringArmComponent->TargetOffset = SpringArmTargetOffset;
    SpringArmComponent->bDoCollisionTest = false;
    SpringArmComponent->SetupAttachment(DefaultSceneRootComponent);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
    CameraComponent->SetupAttachment(SpringArmComponent);

    BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
    BoxComponent->InitBoxExtent(InitBoxExtentValue);
    BoxComponent->SetupAttachment(DefaultSceneRootComponent);

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh Component"));
    StaticMeshComponent->SetupAttachment(BoxComponent);

    bIsGameStart = false;
    bIsSimulatingGameGravity = false;
    bIsJump = false;
    GravityVector = FVector::ZeroVector;

}

// Called when the game starts or when spawned
void AMainPawn::BeginPlay()
{
	Super::BeginPlay();

	ConfigurePawn();

}

// Called every frame
void AMainPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetGravityMechanics(DeltaTime);
    CheckViewportCollision();
    BoxComponentLevitation();
    StartMeshRotationOnFall();

}

// Called to bind functionality to input
void AMainPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	 // Set up action bindings
    if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // Jumping
        EnhancedInputComponent->BindAction(JumpInputAction, ETriggerEvent::Started, this, &AMainPawn::Jump);
    }
}

void AMainPawn::ConfigurePawn()
{
    SetActorRelativeLocation(FVector::ZeroVector);

    BoxComponent->SetSimulatePhysics(true);
    BoxComponent->SetEnableGravity(false);

    StaticMeshComponent->SetSimulatePhysics(false);
    StaticMeshComponent->SetEnableGravity(false);

    // Add OnComponentHit event
    BoxComponent->OnComponentHit.AddDynamic(this, &AMainPawn::OnHit);

    // Add Input Mapping Context
    if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }
}

void AMainPawn::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    // // Disable Input
    // DisableInput(Cast<APlayerController>(Controller));

    // // Get BoxComponents to the OtherActor to disable collision of the pipes
    // TArray<UBoxComponent*> AllPipeBoxComponents;
    // OtherActor->GetComponents<UBoxComponent>(AllPipeBoxComponents);

    // for (UBoxComponent* BoxComponents : AllPipeBoxComponents)
    // {
    //     BoxComponents->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    // }

    // // Enable Gravity on collision
    // BoxComponent->SetEnableGravity(true);

    // // Add force to the actor in location
    // BoxComponent->AddForceAtLocation(NormalImpulse * NormalImpulseForceInCollision, Hit.Location);

    // // Restart the level
    // GetWorldTimerManager().SetTimer(RestartTimerHandle, this, &AFlappy9000Pawn::RestartLevel, RestartLevelRate);
}

void AMainPawn::SetGravityMechanics(float DeltaTime)
{
    if (bIsGameStart)
    {
        if (bIsSimulatingGameGravity)
        {
            if (bIsJump)
            {
                GravityVector.Z = JumpImpulse;

                bIsJump = false;
            }
            else
            {
                GravityVector.Z -= GravityForce * DeltaTime;
            }

            FVector GravityLocation = BoxComponent->GetRelativeLocation() + GravityVector * DeltaTime;
            BoxComponent->SetRelativeLocation(GravityLocation);
        }
    }
}

void AMainPawn::Jump()
{
    if (!bIsGameStart)
    {
        bIsGameStart = true;
    }

    if (!bIsSimulatingGameGravity)
    {
        bIsSimulatingGameGravity = true;
    }

    if (!bIsJump)
    {
        bIsJump = true;
    }

    BoxComponentLocationOnJump = BoxComponent->GetRelativeLocation();

    StartMeshRotationOnJump();
}

void AMainPawn::CheckViewportCollision()
{
    if (bIsGameStart)
    {
        if (!IsBoxComponentInCameraFrustum())
        {
            bIsJump = true;
        }
    }
}

bool AMainPawn::IsBoxComponentInCameraFrustum()
{
    UWorld* World = GetWorld();

    if (!World)
    {
        return false;
    }

    APlayerController* PlayerController = World->GetFirstPlayerController();

    if (!PlayerController)
    {
        return false;
    }

    FVector2D BoxComponentScreenPosition;

    if (!PlayerController->ProjectWorldLocationToScreen(BoxComponent->GetRelativeLocation() - (InitBoxExtentValue.Y / 2), BoxComponentScreenPosition))
    {
        return false;
    }

    const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());

    if (BoxComponentScreenPosition.Y <= ViewportSize.Y)
    {
        return true;
    }

    return false;
}

void AMainPawn::StartMeshRotationOnJump()
{
    FRotator MeshRotationOnJump = StaticMeshComponent->GetRelativeRotation();
    MeshRotationOnJump.Roll = -MeshRotationMaxAngleOnJump;

    StaticMeshComponent->SetRelativeRotation(MeshRotationOnJump);
}

void AMainPawn::StartMeshRotationOnFall()
{
    FVector BoxLocation = BoxComponent->GetRelativeLocation();
    FRotator MeshRotation = StaticMeshComponent->GetRelativeRotation();

    bool bIsFalling = BoxComponentLocationOnJump.Z > BoxLocation.Z;

    if (bIsGameStart)
    {
        if (bIsSimulatingGameGravity)
        {
            if (bIsFalling)
            {
                if (MeshRotation.Roll < (MeshRotationMinAngleOnFall - MeshRotationVelocityOnFalling))
                {
                    MeshRotation.Roll += MeshRotationVelocityOnFalling;
                }
            }

            StaticMeshComponent->SetRelativeRotation(MeshRotation);
        }
    }
}

void AMainPawn::BoxComponentLevitation()
{
    if (!bIsGameStart && !bIsSimulatingGameGravity)
    {
        // Override actor location for levitation
        float TimeSeconds = GetWorld()->GetTimeSeconds();
        float SinValue = (FrequencyLevitation * FMath::Sin(2 * PI * TimeSeconds / PeriodLevitation) * AmplitudeLevitation);

        FVector NewLocationLevitation = BoxComponent->GetRelativeLocation();
        NewLocationLevitation.Z = SinValue;

        BoxComponent->SetRelativeLocation(NewLocationLevitation);
    }
}

void AMainPawn::RestartLevel()
{
    // Restart current level
    FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this, true);
    UGameplayStatics::OpenLevel(this, FName(*CurrentLevelName));
}
