// Fill out your copyright notice in the Description page of Project Settings.


#include "PrimaryPawn.h"
#include "DrawDebugHelpers.h" 
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "../PlayerUI.h"

#define OUT

// Sets default values
APrimaryPawn::APrimaryPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	ballMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));

	RootComponent = ballMesh; 
	springArm->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	springArm->TargetArmLength = 900.f;
	springArm->SetWorldRotation(FRotator(-30.0f, 0.0f, 0.0f));
	camera->AttachToComponent(springArm, FAttachmentTransformRules::KeepWorldTransform, USpringArmComponent::SocketName);	
	// Create an instance of our movement component, and tell it to update the root.
	pawnMovementComponent = CreateDefaultSubobject<UCollidingPawnMovementComponent>(TEXT("PawnCustomMovementComponent"));

	// UI
	ConstructorHelpers::FClassFinder<UUserWidget> WidgetAsset(TEXT("/Game/UI/PlayerUI"));
	if( WidgetAsset.Succeeded() ){	// Find PlayeUI blueprint and store a refrence to the class (create instance in BeginPlay)
		PlayerUIClass = WidgetAsset.Class;		
	}else {
		UE_LOG(LogTemp, Error, TEXT("Could not get Find Class UPlayerUI."));
	}
	AutoPossessPlayer = EAutoReceiveInput::Player0; 
}

UPawnMovementComponent* APrimaryPawn::GetMovementComponent() const{
    return pawnMovementComponent;
}

// Called when the game starts or when spawned
void APrimaryPawn::BeginPlay() 
{
	Super::BeginPlay();
	pawnMovementComponent->UpdatedComponent = RootComponent;
	controller = GetWorld()->GetFirstPlayerController();
	if(!controller){
		UE_LOG(LogTemp, Error, TEXT("Could not get player controller."));
	}else{
		UE_LOG(LogTemp, Warning, TEXT("Wee we have a player controller"));
	}

	if( PlayerUIClass ){         
		 mPlayerUI = CreateWidget<UPlayerUI>(GetWorld()->GetFirstPlayerController(), PlayerUIClass);                  
         if( mPlayerUI ){   // Make sure widget was created           
             mPlayerUI->AddToViewport( );
         }else{
			 UE_LOG(LogTemp, Error, TEXT("Could not get create instance of PlayerUI."));
		 }
     }
}

// Called every frame
void APrimaryPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// TODO: if player below some thresh in Z axis.. they are dead
	if(mode == PawnMode::Camera){
		FRotator yawUpdate = springArm->GetComponentRotation();
		FVector2D delta = mTouchMoved - mTouchBegin;	
		yawUpdate.Yaw -= delta.X / 8.0f;	// 8? dampening?
		springArm->SetWorldRotation(yawUpdate);
		mTouchBegin = mTouchMoved;
	}else if(mode == PawnMode::Aim){
		//FHitResult hit;
		//FCollisionQueryParams traceParams(FName(TEXT("")), false, GetOwner());
		//controller->GetHitResultAtScreenPosition(singleTap, ECollisionChannel::ECC_Pawn, traceParams, OUT hit);
		FVector ballLocation = GetActorLocation();
		FVector WorldLoc, WorldDir;
		if(controller->DeprojectScreenPositionToWorld(mTouchMoved.X, mTouchMoved.Y, WorldLoc, WorldDir)){
			// FIXME: is there a better way then making 'WorldDir*5e10f' really long?
			FVector hit = FMath::LinePlaneIntersection(WorldLoc, WorldDir*5e10f, FPlane(ballLocation, FVector(0.0f, 0.0f, 1.0f)) );
			//UE_LOG(LogTemp, Warning, TEXT("%s PLANE HIT: %s %s"), *mTouchMoved.ToString(), *ballLocation.ToString(), *hit.ToString());
			DrawDebugLine(GetWorld(), ballLocation, hit, FColor::Red, false, -1.f, 0, 2.0f);
			mAimVector = ballLocation - hit;
		} 
	}
} 

// Called to bind functionality to input
void APrimaryPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APrimaryPawn::TouchBegin(const FVector2D &touchBegin){ 
	//UE_LOG(LogTemp, Warning, TEXT("TouchBegin"));
	mTouchBegin = touchBegin;
	mTouchMoved = touchBegin;
	UpdateTouchBegin(touchBegin);
}

void APrimaryPawn::TouchEnd(const FVector2D &touchEnd){
	//UE_LOG(LogTemp, Warning, TEXT("TouchEnd"));
	UpdateTouchEnd(touchEnd);
	mTouchBegin = FVector2D(0.f, 0.f);		// reset the vector 
	mTouchMoved = FVector2D(0.f, 0.f);
}

void APrimaryPawn::TouchMoved(const FVector2D &touchMoved){
	//UE_LOG(LogTemp, Warning, TEXT("TouchMove"));
	mTouchMoved = touchMoved;
	if(mode == PawnMode::Selected){
		mode = PawnMode::Aim;

	}
}

void APrimaryPawn::SingleTap(const FVector2D &singleTap){
	//UE_LOG(LogTemp, Warning, TEXT("TouchEnd: %s"), *touchBegin.ToString());
	UE_LOG(LogTemp, Warning, TEXT("SingleTap !"));
	UpdateTouchBegin(singleTap); 	
}

 
void APrimaryPawn::UpdateTouchBegin(const FVector2D &touch){
	// reset needed vars..
	mAimVector = GetActorLocation();
	if(controller){
		FHitResult hit;
		FCollisionQueryParams traceParams(FName(TEXT("")), false, GetOwner());
		controller->GetHitResultAtScreenPosition(touch, ECollisionChannel::ECC_Pawn, traceParams, OUT hit);
		if(hit.GetActor() && hit.GetActor()->GetUniqueID() == this->GetUniqueID()){
			UE_LOG(LogTemp, Warning, TEXT("APrimaryPawn Hit : %s"), *hit.GetActor()->GetName() );
			mode = PawnMode::Selected;
		}else{
			// TODO: check other hits
			mode = PawnMode::Camera; 
		}
	}
} 

void APrimaryPawn::UpdateTouchEnd(const FVector2D &touch){
	if(mode == PawnMode::Aim){
		if (pawnMovementComponent && (pawnMovementComponent->UpdatedComponent == RootComponent)){
			// TODO: we want the camera look vector projected onto the groud plane...
			// VectorPlaneProject
			// http://api.unrealengine.com/INT/API/Runtime/Core/Math/FVector/index.html
			FVector look = camera->GetComponentRotation().Vector();
			//FVector thrust = FVector::VectorPlaneProject(look, FVector(0.0f, 0.0f, 1.0f)) * 4000.0f;
			FVector thrust = mAimVector * 15.0f;	// TODO: scale by wtf?
			UE_LOG(LogTemp, Warning, TEXT("Move forward !!!!!!!!!!!!!!!!   %s"), *thrust.ToString() );
			pawnMovementComponent->AddInputVector(thrust);			
		}
		mode = PawnMode::Camera;
	}
}

