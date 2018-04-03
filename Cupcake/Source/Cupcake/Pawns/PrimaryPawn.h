// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/MeshComponent.h"
#include "PrimaryPawn.generated.h"

UCLASS()
class CUPCAKE_API APrimaryPawn : public APawn
{
	GENERATED_BODY() 

public:
	// Sets default values for this pawn's properties
	APrimaryPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UCameraComponent* camera = nullptr;
	FVector2D touchInput;
	
	void DragX(float x);
	void DragY(float y);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* mesh = nullptr;

	UPROPERTY(EditAnywhere)
		USpringArmComponent* springArm = nullptr;

	
	
};
