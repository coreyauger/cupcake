// Fill out your copyright notice in the Description page of Project Settings.


#include "PrimaryPawn.h"
 

// Sets default values
APrimaryPawn::APrimaryPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	RootComponent = mesh; 
	springArm->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	springArm->TargetArmLength = 2000.f;
	springArm->SetWorldRotation(FRotator(-45.0f, 0.0f, 0.0f));

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
//	PlayerInputComponent->BindAxis("DragX", this, &APrimaryPawn::DragX);
//	PlayerInputComponent->BindAxis("DragY", this, &APrimaryPawn::DragY);
}

void APrimaryPawn::TouchBegin(const FVector2D &touchBegin){
	//UE_LOG(LogTemp, Warning, TEXT("TouchBegin: %s"), *touchBegin.ToString());
	UE_LOG(LogTemp, Warning, TEXT("TouchBegin"));
}

void APrimaryPawn::TouchEnd(const FVector2D &touchEnd){
	//UE_LOG(LogTemp, Warning, TEXT("TouchEnd: %s"), *touchBegin.ToString());
	UE_LOG(LogTemp, Warning, TEXT("TouchEnd"));
}

void APrimaryPawn::TouchMoved(const FVector2D &touchMoved){
	//UE_LOG(LogTemp, Warning, TEXT("TouchEnd: %s"), *touchBegin.ToString());
	UE_LOG(LogTemp, Warning, TEXT("TouchMove"));
}

void APrimaryPawn::SingleTap(const FVector2D &singleTap){
	//UE_LOG(LogTemp, Warning, TEXT("TouchEnd: %s"), *touchBegin.ToString());
	UE_LOG(LogTemp, Warning, TEXT("SingleTap"));
}

void APrimaryPawn::DragX(float x){
	UE_LOG(LogTemp, Warning, TEXT("DragX: %f"), x);
	touchInput.X = x;
}

void APrimaryPawn::DragY(float y){
	UE_LOG(LogTemp, Warning, TEXT("DragY: %f"), y);
	touchInput.Y = y;
}

