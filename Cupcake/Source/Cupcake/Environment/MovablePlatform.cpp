// Fill out your copyright notice in the Description page of Project Settings.


#include "MovablePlatform.h"
#include "ConstructorHelpers.h"
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
	FOnTimelineFloat onTimelineCallback;
	FOnTimelineEventStatic onTimelineFinishedCallback;
 
	Super::BeginPlay();

	mesh = GetOwner()->FindComponentByClass<UStaticMeshComponent>();  
	StartLocation = GetOwner()->GetActorLocation();
	UE_LOG(LogTemp, Warning, TEXT("UMovablePlatform::BeginPlay %s"), *GetOwner()->GetActorLocation().ToString());
 
	UE_LOG(LogTemp, Warning, TEXT("UMovablePlatform::BeginPlay"));
	if (FloatCurve != NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT("Got FloatCurve"));
		MyTimeline = NewObject<UTimelineComponent>(this, FName("TimelineAnimation"));
		MyTimeline->CreationMethod = EComponentCreationMethod::UserConstructionScript; // Indicate it comes from a blueprint so it gets cleared when we rerun construction scripts
		
		// CA - this is only on a UActor
		//this->BlueprintCreatedComponents.Add(MyTimeline); // Add to array so it gets saved
		MyTimeline->SetNetAddressable();	// This component has a stable name that can be referenced for replication
 
		MyTimeline->SetPropertySetObject(this); // Set which object the timeline should drive properties on
		MyTimeline->SetDirectionPropertyName(FName("TimelineDirection"));
 
		MyTimeline->SetLooping(Looping);
		MyTimeline->SetTimelineLength(5.0f);
		MyTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_LastKeyFrame);
 
		MyTimeline->SetPlaybackPosition(0.0f, false);
 
		//Add the float curve to the timeline and connect it to your timelines's interpolation function
		onTimelineCallback.BindUFunction(this, FName{ TEXT("TimelineCallback") });
		onTimelineFinishedCallback.BindUFunction(this, FName{ TEXT("TimelineFinishedCallback") });
		MyTimeline->AddInterpFloat(FloatCurve, onTimelineCallback);
		MyTimeline->SetTimelineFinishedFunc(onTimelineFinishedCallback);
 
		MyTimeline->RegisterComponent();
	}else{
		UE_LOG(LogTemp, Warning, TEXT("NO FloatCurve for MovablePlatform"));
	}

	if(AutoPlay)PlayTimeline();
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
	if (MyTimeline != NULL){
		MyTimeline->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, NULL);
	}
}


void UMovablePlatform::TimelineCallback(float interpolatedVal)
{
	UE_LOG(LogTemp, Warning, TEXT("TimelineCallback"));
	FVector movement = FinalLocation - StartLocation;
	FVector newLocation = StartLocation + (movement * interpolatedVal);
	UE_LOG(LogTemp, Warning, TEXT("interpolatedVal: %f"), interpolatedVal );
	UE_LOG(LogTemp, Warning, TEXT("newLocation: %s"), *newLocation.ToString() );
	GetOwner()->SetActorLocation(newLocation);

    // This function is called for every tick in the timeline.
}
 
void UMovablePlatform::TimelineFinishedCallback()
{
	UE_LOG(LogTemp, Warning, TEXT("TimelineFinishedCallback"));
}

void UMovablePlatform::PlayTimelineReverse()
{
	UE_LOG(LogTemp, Warning, TEXT("PlayTimelineReverse"));
	if (MyTimeline != NULL){
		UE_LOG(LogTemp, Warning, TEXT("Reverse"));
		MyTimeline->ReverseFromEnd();
	}
}
 
void UMovablePlatform::PlayTimeline()
{
	UE_LOG(LogTemp, Warning, TEXT("PlayTimeline"));
	if (MyTimeline != NULL){
		UE_LOG(LogTemp, Warning, TEXT("PlayFromStart"));
		MyTimeline->PlayFromStart();
	}
}

