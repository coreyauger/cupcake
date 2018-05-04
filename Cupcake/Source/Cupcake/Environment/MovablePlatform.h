// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "MovablePlatform.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CUPCAKE_API UMovablePlatform : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties 
	UMovablePlatform();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	class UStaticMeshComponent* mesh;

	struct TimeLocationPair{
		float Seconds;
		FVector Location;
		TimeLocationPair(float sec, FVector location): Seconds(sec), Location(location)
		{}
		TimeLocationPair(){}
	};

	TMap<FString, TimeLocationPair> LastLocationMap;

	/// timeline functionality
	UPROPERTY()
    class UTimelineComponent* MyTimeline = nullptr;
 

 	/** MovmentCurve Curve */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MovmentCurve")
	class UCurveFloat* FloatCurve = nullptr;
 
    UFUNCTION()
    void TimelineCallback(float val);
 
    UFUNCTION()
    void TimelineFinishedCallback();
 
    void PlayTimeline();
	void PlayTimelineReverse();
 
    UPROPERTY()
    TEnumAsByte<ETimelineDirection::Type> TimelineDirection;   	
	

public:	 
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
	bool AutoPlay = false;

	UPROPERTY(EditAnywhere)
	bool Looping = false;

	UPROPERTY(EditAnywhere)
	FRotator FinalRotation = FRotator(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere)
	FVector FinalLocation = FVector(0.0f, 0.0f, 0.0f);
	// StartLoation gets assigned Actor Location OnBeginPlay
	FVector StartLocation = FVector(0.0f, 0.0f, 0.0f); 

	/** What is the final Rotation? ! */
	UFUNCTION(BlueprintPure, Category="Movment")
	FRotator GetFinalRotation(){return FinalRotation;};
		
	/** What is the final Position? ! */
	UFUNCTION(BlueprintPure, Category="Movment")
	FVector GetFinalPosition(){return FinalLocation;};

	//FVector LastTickLocation = FVector(0.0f, 0.0f, 0.0f);

	FVector GetThenSetLastLocation(const FString& ActorName);

	UPROPERTY(EditAnywhere, Category = "Audio")
    class USoundCue* movableAudioCue;


    UPROPERTY(EditAnywhere, Category = "Audio")
    class UAudioComponent* movableAudioComponent;	
};
 