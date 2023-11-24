// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PipeActor.generated.h"

UCLASS()
class A001U_API APipeActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APipeActor();

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "PipeActor", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DefaultSceneRootComponent;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "PipeActor", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* MainSceneComponent;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "PipeActor", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* PipeMeshComponent;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "PipeActor", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* CapPipeMeshComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
    void MovePipe(float DeltaTime);
	
};
