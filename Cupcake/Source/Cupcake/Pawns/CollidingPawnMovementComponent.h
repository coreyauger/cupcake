// Fill out your copyright notice in the Description page of Project Settings.

#pragma once 

#include "CoreMinimal.h" 
#include "GameFramework/PawnMovementComponent.h" 
#include "Camera/CameraComponent.h"
#include "Components/MeshComponent.h"
#include "CollidingPawnMovementComponent.generated.h" 
   
//https://stackoverflow.com/questions/8063696/arkanoid-physics-projectile-physics-simulation
/** 
 *    
 */  
UCLASS() 
class CUPCAKE_API UCollidingPawnMovementComponent : public UPawnMovementComponent
{  
	GENERATED_BODY()   
    
private:  
   
public:
    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;  
    virtual void HandleImpact(const FHitResult& Hit, float TimeSlice=0.f, const FVector& MoveDelta = FVector::ZeroVector) override;  

    // TODO: make these a level setting..
    UPROPERTY(EditAnywhere) 
	float gravity = -98.1f;

    UPROPERTY(EditAnywhere) 
	float drag = 0.025f; 
  
    // mass in Kg
    UPROPERTY(EditAnywhere) 
	float mass = 10.0f;	
};  
 