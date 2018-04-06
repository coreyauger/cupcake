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
    Velocity = (Velocity /*+ gravity*/ + ConsumeInputVector()) * (1.0f-drag);

    // Get (and then clear) the movement vector that we set in ACollidingPawn::Tick
    //FVector DesiredMovementThisFrame = ConsumeInputVector().GetClampedToMaxSize(1.0f) * DeltaTime * 15000.0f;
    FVector desiredMovementThisFrame = Velocity * DeltaTime;

    if (!desiredMovementThisFrame.IsNearlyZero()){
        FHitResult Hit;
        SafeMoveUpdatedComponent(desiredMovementThisFrame, UpdatedComponent->GetComponentRotation(), true, Hit);
        //UE_LOG(LogTemp, Warning, TEXT("DO MOVE: %s"), *desiredMovementThisFrame.ToString() );
        // If we bumped into something, try to slide along it
        if (Hit.IsValidBlockingHit()){ 

            Velocity = Velocity.MirrorByVector(Hit.ImpactNormal);
            Velocity = FVector(Velocity.X, Velocity.Y, 0.0f);
            
            DesiredMovementThisFrame = Velocity * DeltaTime;
            // TODO.. need to check hits again.

            //SafeMoveUpdatedComponent(desiredMovementThisFrame, UpdatedComponent->GetComponentRotation(), true, Hit);
            //MoveUpdatedComponent(desiredMovementThisFrame, UpdatedComponent->GetComponentRotation(), true, &Hit);
            
            // Allow this hit to be used as an impact we can deflect off, otherwise we do nothing the rest of the update and appear to hitch.
            HandleImpact(Hit);
            //SlideAlongSurface(desiredMovementThisFrame, 1.f - Hit.Time, Hit.Normal, Hit);
//UE_LOG(LogTemp, Warning, TEXT("NORM: %s"), *Hit.ImpactNormal.ToString() );
UE_LOG(LogTemp, Warning, TEXT("REFLECT: %s"), *Velocity.MirrorByVector(Hit.ImpactNormal).ToString() );
            UpdatedComponent->MoveComponent(desiredMovementThisFrame, UpdatedComponent->GetComponentRotation(), true);

            //GetOwner()->SetActorLocation(GetOwner()->GetActorLocation() + DesiredMovementThisFrame);
        } 
         

       //Get the static mesh of the chosen Actor
       // UStaticMeshComponent* SM = GetOwner()->FindComponentByClass<UStaticMeshComponent>();
    }
};



