// Fill out your copyright notice in the Description page of Project Settings.

#include "CollidingPawnMovementComponent.h"

void UCollidingPawnMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Make sure that everything is still valid, and that we are allowed to move.
    if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime)){
        return;
    }

    // simulate physics...
    Velocity = (Velocity + gravity + ConsumeInputVector()) * (1.0f-drag);

    // Get (and then clear) the movement vector that we set in ACollidingPawn::Tick
    //FVector DesiredMovementThisFrame = ConsumeInputVector().GetClampedToMaxSize(1.0f) * DeltaTime * 15000.0f;
    FVector DesiredMovementThisFrame = Velocity * DeltaTime;

    if (!DesiredMovementThisFrame.IsNearlyZero()){
        FHitResult Hit;
        SafeMoveUpdatedComponent(DesiredMovementThisFrame, UpdatedComponent->GetComponentRotation(), true, Hit);
        //UE_LOG(LogTemp, Warning, TEXT("DO MOVE: %s"), *DesiredMovementThisFrame.ToString() );
        // If we bumped into something, try to slide along it
        if (Hit.IsValidBlockingHit()){
            
            //DesiredMovementThisFrame = -Velocity.MirrorByVector(Hit.ImpactNormal);
            //SafeMoveUpdatedComponent(DesiredMovementThisFrame, UpdatedComponent->GetComponentRotation(), true, Hit);
            //MoveUpdatedComponent(DesiredMovementThisFrame, UpdatedComponent->GetComponentRotation(), true, &Hit);
            SlideAlongSurface(DesiredMovementThisFrame, 1.f - Hit.Time, Hit.Normal, Hit);
            //GetOwner()->SetActorLocation(GetOwner()->GetActorLocation() + DesiredMovementThisFrame);
        } 
         

       //Get the static mesh of the chosen Actor
       // UStaticMeshComponent* SM = GetOwner()->FindComponentByClass<UStaticMeshComponent>();
    }
};



