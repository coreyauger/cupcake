// Fill out your copyright notice in the Description page of Project Settings.


#include "MovablePlatform.h"
#include <Kismet/GameplayStatics.h> 

// Sets default values for this component's properties
UMovablePlatform::UMovablePlatform()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	//PrimaryComponentTick.TickGroup = TG_PrePhysics;
 
 	// make sure we default the mesh to movable	
} 


// Called when the game starts
void UMovablePlatform::BeginPlay()
{
	Super::BeginPlay();
	// ...
	mesh = GetOwner()->FindComponentByClass<UStaticMeshComponent>();  
	//if(mesh)LastTickLocation = mesh->GetComponentLocation();
}

FVector UMovablePlatform::GetThenSetLastLocation(const FString& ActorName){
	// Lookup pawn name and get the last Time of a location
	if(!LastLocationMap.Contains(ActorName)){
		FVector location = mesh->GetComponentLocation();
		float realtimeSeconds = UGameplayStatics::GetRealTimeSeconds(GetWorld());
		LastLocationMap.Add(ActorName, UMovablePlatform::TimeLocationPair(realtimeSeconds, location) );
		return location;
	}else{
		UMovablePlatform::TimeLocationPair val = LastLocationMap.FindRef(ActorName);
		float realtimeSeconds = UGameplayStatics::GetRealTimeSeconds(GetWorld());
		if(val.Seconds - realtimeSeconds < 1.0f){
			LastLocationMap.Add(ActorName, UMovablePlatform::TimeLocationPair(realtimeSeconds, mesh->GetComponentLocation()) );
			return val.Location;
		}else{
			FVector location = mesh->GetComponentLocation();
			LastLocationMap.Add(ActorName, UMovablePlatform::TimeLocationPair(realtimeSeconds, location) );
			return location;
		}
	}
}


// Called every frame
void UMovablePlatform::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

