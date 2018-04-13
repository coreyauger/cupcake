// Fill out your copyright notice in the Description page of Project Settings.


#include "MovablePlatform.h"
 

// Sets default values for this component's properties
UMovablePlatform::UMovablePlatform()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = TG_PrePhysics;
 
 	// make sure we default the mesh to movable	
}


// Called when the game starts
void UMovablePlatform::BeginPlay()
{
	Super::BeginPlay();
	// ...
	mesh = GetOwner()->FindComponentByClass<UStaticMeshComponent>();  
	if(mesh)LastTickLocation = mesh->GetComponentLocation();
}


// Called every frame
void UMovablePlatform::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(mesh){
		// HACK: this is used in CollidingPawn to allowo it to stay fixed to a movable floor
		LastTickLocation = mesh->GetComponentLocation();
		//UE_LOG(LogTemp, Warning, TEXT("=== UPDATE: %s"), *LastTickLocation.ToString() ); 
	}
}

