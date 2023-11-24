// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PipeSpawnerActor.generated.h"

UENUM(BlueprintType)
enum EPipeType : uint8
{
	Up,
	Down,
};

UCLASS()
class A001U_API APipeSpawnerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APipeSpawnerActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
    void SpawnPipes();

	void SetPipeScale3DTransformations(AActor* PipeActor);

	FVector GetPipeSpawnLocation(EPipeType PipeType);
	FRotator GetPipeSpawnRotation(EPipeType PipeType);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AActor> PipeActorClass;

	FTimerHandle TimerHandle;
	
};
