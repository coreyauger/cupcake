// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerVolume.h"
#include "SpeedBoostTriggerVolume.generated.h"

/**
 * 
 */
UCLASS()
class CUPCAKE_API ASpeedBoostTriggerVolume : public ATriggerVolume
{
	GENERATED_BODY()
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
	
public:

	// constructor sets default values for this actor's properties
	ASpeedBoostTriggerVolume();

	// overlap begin function
	UFUNCTION()
	void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);

	// overlap end function
	UFUNCTION()
	void OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor);
	
	
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* SpeedBoostMesh = nullptr; 

};
