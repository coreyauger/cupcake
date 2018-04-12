// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MovablePlatform.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMovmentTransition, int, State);

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

	UPROPERTY(BlueprintAssignable)
	FOnMovmentTransition OnMovmentTransition; 

	UPROPERTY(BlueprintAssignable)
	FOnMovmentTransition OnReturnTransition; 

public:	 
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
	bool AutoTransition = false;

	UPROPERTY(EditAnywhere)
	FRotator FinalRotation = FRotator(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere)
	FVector FinalLocation = FVector(0.0f, 0.0f, 0.0f);

	/** What is the final Rotation? ! */
	UFUNCTION(BlueprintPure, Category="Movment")
	FRotator GetFinalRotation(){return FinalRotation;};
		
	/** What is the final Position? ! */
	UFUNCTION(BlueprintPure, Category="Movment")
	FVector GetFinalPosition(){return FinalLocation;};
};
 