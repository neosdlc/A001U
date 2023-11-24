// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MainPawn.generated.h"

UCLASS()
class A001U_API AMainPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMainPawn();

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpInputAction;

	/** Box Component */
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "AMainPawn", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BoxComponent;

	/** Static Mesh Component */
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "AMainPawn", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* StaticMeshComponent;

	/** Scene Component */
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "AMainPawn", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DefaultSceneRootComponent;

	/** SpringArm Component */
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArmComponent;

	/** Camera Component */
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
    UFUNCTION()
	void OnHit(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit);

	void ConfigurePawn();

    void BoxComponentLevitation();

    void Jump();

	void StartMeshRotationOnFall();

	void StartMeshRotationOnJump();

	void RestartLevel();

	void SetGravityMechanics(float DeltaTime);

	bool IsBoxComponentInCameraFrustum();

	void CheckViewportCollision();

	bool bIsGameStart;

	bool bIsSimulatingGameGravity;

	bool bIsJump;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Values", meta = (AllowPrivateAccess = "true"))
	float FrequencyLevitation = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Values", meta = (AllowPrivateAccess = "true"))
	float AmplitudeLevitation = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Values", meta = (AllowPrivateAccess = "true"))
	float PeriodLevitation = 1.5f;

	float JumpImpulse = 800.0f;

	float GravityForce = 3210.0f;

	float SpringArmTargetArmLength = 300.0f;

	float NormalImpulseForceInCollision = 30.0f;

	float MeshRotationVelocityOnFalling = 5.0f;

	float MeshRotationMaxAngleOnJump = 21.0f;

	float MeshRotationMinAngleOnFall = 90.0f;

	FVector InitBoxExtentValue = FVector(2.0f, 34.0f, 24.0f);

	FVector SpringArmTargetOffset = FVector(0.0f, 0.0f, 0.0f);

	FVector GravityVector;

	FVector BoxComponentLocationOnJump;
	
};
