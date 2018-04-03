// Fill out your copyright notice in the Description page of Project Settings.


#include "PrimaryPawn.h"


// Sets default values
APrimaryPawn::APrimaryPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	mesh = CreateDefaultSubobject<UMeshComponent>(TEXT("Mesh"));

	RootComponent = mesh;
	springArm->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	springArm->TargetArmLength = 2500.f;
	springArm->SetWorldRotation(FRotator(-50.0f, 0.0f, 0.0f));

	camera->AttachToComponent(springArm, FAttachmentTransformRules::KeepWorldTransform, USpringArmComponent::SocketName);

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void APrimaryPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APrimaryPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APrimaryPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

