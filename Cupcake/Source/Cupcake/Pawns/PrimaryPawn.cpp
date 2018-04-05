// Fill out your copyright notice in the Description page of Project Settings.


#include "PrimaryPawn.h"
 

#define OUT

// Sets default values
APrimaryPawn::APrimaryPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	//RootComponent = mesh; 
	springArm->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	springArm->TargetArmLength = 2000.f;
	springArm->SetWorldRotation(FRotator(-45.0f, 0.0f, 0.0f));

	camera->AttachToComponent(springArm, FAttachmentTransformRules::KeepWorldTransform, USpringArmComponent::SocketName);

	// Create an instance of our movement component, and tell it to update the root.
	pawnMovementComponent = CreateDefaultSubobject<UCollidingPawnMovementComponent>(TEXT("PawnCustomMovementComponent"));
	pawnMovementComponent->UpdatedComponent = RootComponent;

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

UPawnMovementComponent* APrimaryPawn::GetMovementComponent() const{
    return pawnMovementComponent;
}

// Called when the game starts or when spawned
void APrimaryPawn::BeginPlay()
{
	Super::BeginPlay();
	controller = GetWorld()->GetFirstPlayerController();
	if(!controller){
		UE_LOG(LogTemp, Error, TEXT("Could not get player controller."));
	}else{
		UE_LOG(LogTemp, Warning, TEXT("Wee we have a player controller"));
	}
}

// Called every frame
void APrimaryPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FRotator yawUpdate = springArm->GetComponentRotation();
	FVector2D delta = mTouchMoved - mTouchBegin;	
	yawUpdate.Yaw -= delta.X / 8.0f;	// 8? dampening?
	springArm->SetWorldRotation(yawUpdate);
	mTouchBegin = mTouchMoved;
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
	mTouchBegin = touchBegin;
	mTouchMoved = touchBegin;
	// TODO: do a hit test for our pawn.
	// - if we hit the pawn then we switch into "shot" mode.
}

void APrimaryPawn::TouchEnd(const FVector2D &touchEnd){
	//UE_LOG(LogTemp, Warning, TEXT("TouchEnd: %s"), *touchBegin.ToString());
	UE_LOG(LogTemp, Warning, TEXT("TouchEnd"));
	mTouchBegin = FVector2D(0.f, 0.f);		// reset the vector 
	mTouchMoved = FVector2D(0.f, 0.f);
}

void APrimaryPawn::TouchMoved(const FVector2D &touchMoved){
	//UE_LOG(LogTemp, Warning, TEXT("TouchEnd: %s"), *touchBegin.ToString());
	UE_LOG(LogTemp, Warning, TEXT("TouchMove"));
	mTouchMoved = touchMoved;
}

void APrimaryPawn::SingleTap(const FVector2D &singleTap){
	//UE_LOG(LogTemp, Warning, TEXT("TouchEnd: %s"), *touchBegin.ToString());
	UE_LOG(LogTemp, Warning, TEXT("SingleTap !"));
	if(controller){
		UE_LOG(LogTemp, Warning, TEXT("Hit Test !!"));
		FHitResult hit;
		FCollisionQueryParams traceParams(FName(TEXT("")), false, GetOwner());
		controller->GetHitResultAtScreenPosition(singleTap, ECollisionChannel::ECC_Pawn, traceParams, OUT hit);
		if(hit.GetActor()){
			UE_LOG(LogTemp, Warning, TEXT("UGrabber Hit : %s"), *hit.GetActor()->GetName() );
			// TODO: this could be used to selct or query items in the scene..
			//GetOwner()->GetActor()->
			// TODO: trying to move our pawn with a click
			if (pawnMovementComponent && (pawnMovementComponent->UpdatedComponent == RootComponent)){
				UE_LOG(LogTemp, Warning, TEXT("Move forward !!!!!!!!!!!!!!!!") );
				// TODO: we want the camera look vector projected onto the groud plane...
				FVector look = camera->GetComponentRotation().Vector();
				pawnMovementComponent->AddInputVector(look * 10000.0f);	// NOTE: this is just scaled between 0-1
			}
		}
	}
}

