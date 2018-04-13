// Fill out your copyright notice in the Description page of Project Settings.

#include "CollidingPawnMovementComponent.h"
#include "../Environment/MovablePlatform.h"



void UCollidingPawnMovementComponent::HandleImpact(const FHitResult& Hit, float TimeSlice, const FVector& MoveDelta){    
    if(Hit.IsValidBlockingHit()){
        if(Hit.GetActor()){
            auto* mesh = Hit.GetActor()->FindComponentByClass<UStaticMeshComponent>();  
            auto* movablePlatform = Hit.GetActor()->FindComponentByClass<UMovablePlatform>();            
            //if(mesh && mesh->Mobility == EComponentMobility::Type::Movable){                      
            //}
            if(movablePlatform){
                //FBoxSphereBounds bounds = UpdatedComponent->Bounds;                   
                //UE_LOG(LogTemp, Warning, TEXT("** WWW: %f"),  bounds.GetSphere().W ); 
                FVector BallLocation = UpdatedComponent->GetComponentLocation();
                //const float BALL_RADIUS = 2.0;  // SOMETHING IS FUXT HERE..
                // NOTE: we only need the X, Y components to allow the ball to say focused on the platform.
                FVector objVector = BallLocation - mesh->GetComponentLocation();
                FVector finalObjectLocation = movablePlatform->FinalLocation + objVector;
                FVector objectPath = finalObjectLocation - BallLocation;                    
                
                //UE_LOG(LogTemp, Warning, TEXT("** OBJ: %s"), *objVector.ToString() ); 
                UE_LOG(LogTemp, Warning, TEXT("** PLT: %s"), *mesh->GetComponentLocation().ToString() );
                UE_LOG(LogTemp, Warning, TEXT("** LST: %s"), *movablePlatform->LastTickLocation.ToString() );
            
                //UE_LOG(LogTemp, Warning, TEXT("** HIT: %s"), *Hit.Location.ToString() ); 
                //UE_LOG(LogTemp, Warning, TEXT("** Loc: %s"), *BallLocation.ToString() ); 
                //UE_LOG(LogTemp, Warning, TEXT("** FIN: %s"), *finalObjectLocation.ToString() ); 
                if(!objectPath.Normalize(0.0001))return;
                FVector dist = movablePlatform->LastTickLocation - mesh->GetComponentLocation();
                //UE_LOG(LogTemp, Warning, TEXT("** DDD: %s"), *dist.ToString() );
                //UE_LOG(LogTemp, Warning, TEXT("** DIS: %f"), dist.Size() );
                //UE_LOG(LogTemp, Warning, TEXT("** TSS: %f"), TimeSlice );
                FVector move = objectPath * dist.Size();
                //UE_LOG(LogTemp, Warning, TEXT("** DEL: %s"), *MoveDelta.ToString() ); 
                //UE_LOG(LogTemp, Warning, TEXT("** MOV: %s"), *move.ToString() ); 
                //UE_LOG(LogTemp, Warning, TEXT("===================================================="));
                
                FHitResult Hit2;
                SafeMoveUpdatedComponent(move, UpdatedComponent->GetComponentRotation(), true, Hit2);
                movablePlatform->LastTickLocation = mesh->GetComponentLocation();
                //UpdatedComponent->MoveComponent(FVector(move.X, move.Y, 0.0f), UpdatedComponent->GetComponentRotation(), true); 
            }  
        }
    }

}


void UCollidingPawnMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Make sure that everything is still valid, and that we are allowed to move.
    if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime)){
        return;
    }

    // simulate physics... 
    //https://gamedev.stackexchange.com/questions/15708/how-can-i-implement-gravity
    // we want to first check if we are on the ground and if so lets NOT add gravity to the equation    
    FHitResult FloorHit;
    FVector gravityToApply = FVector(0.0f, 0.0f, 0.0f);
    if(SafeMoveUpdatedComponent(FVector(0.0f, 0.0f, -15.0f), UpdatedComponent->GetComponentRotation(), true, FloorHit)){
        if(!FloorHit.Normal.Equals(FVector(0.0f, 0.0f, 1.0f), 0.1f) )gravityToApply.Z += gravity;     
        HandleImpact(FloorHit);              
    }else{
        gravityToApply.Z += gravity;
    }
    Velocity = (Velocity + gravityToApply + ConsumeInputVector()) * (1.0f-drag);

    // Get (and then clear) the movement vector that we set in ACollidingPawn::Tick
    //FVector DesiredMovementThisFrame = ConsumeInputVector().GetClampedToMaxSize(1.0f) * DeltaTime * 15000.0f;
    FVector desiredMovementThisFrame = Velocity * DeltaTime; 

    if (!desiredMovementThisFrame.IsNearlyZero()){ 
        FHitResult Hit;
        SafeMoveUpdatedComponent(desiredMovementThisFrame, UpdatedComponent->GetComponentRotation(), true, Hit);
        //UE_LOG(LogTemp, Warning, TEXT("DO MOVE: %s"), *desiredMovementThisFrame.ToString() );
        // If we bumped into something, try to slide along it
        if (Hit.IsValidBlockingHit()){ 
            FVector currentLocation = UpdatedComponent->GetComponentLocation();
            FVector movementVector = Hit.Location - currentLocation;
             // find out how much of the desired movment gets "eaten up" before the reflection
            float sizeOfReflect = desiredMovementThisFrame.Size() - movementVector.Size();
            if(!movementVector.Normalize(0.0001)){
                // Hmmm is this ok?
                movementVector = desiredMovementThisFrame;
                if(!movementVector.Normalize(0.0001)){ // no movment.. so just return?
                    return;
                }
            } 
            FVector reflect = movementVector.MirrorByVector(Hit.ImpactNormal);
            reflect.Normalize(); 
            desiredMovementThisFrame = reflect * sizeOfReflect;
            // *******
           // TODO.. need to check hits again.

            Velocity = Velocity.MirrorByVector(Hit.ImpactNormal).ProjectOnTo(reflect);   //
            //Velocity = FVector(Velocity.X, Velocity.Y, 0.0f);
            
    
            

            //SafeMoveUpdatedComponent(desiredMovementThisFrame, UpdatedComponent->GetComponentRotation(), true, Hit);
            //MoveUpdatedComponent(desiredMovementThisFrame, UpdatedComponent->GetComponentRotation(), true, &Hit);
            
            // Allow this hit to be used as an impact we can deflect off, otherwise we do nothing the rest of the update and appear to hitch.
            HandleImpact(Hit);
            //UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *Hit.Normal.ToString() );
            //if( Hit.Normal.Equals(FVector(0.0f, 0.0f, 1.0f), 0.1f) ){
            //SlideAlongSurface(desiredMovementThisFrame, 1.f - Hit.Time, Hit.Normal, Hit);
            
            //UE_LOG(LogTemp, Warning, TEXT("REFLECT: %s"), *Velocity.MirrorByVector(Hit.ImpactNormal).ToString() );
            UpdatedComponent->MoveComponent(desiredMovementThisFrame, UpdatedComponent->GetComponentRotation(), true);
            

            //GetOwner()->SetActorLocation(GetOwner()->GetActorLocation() + DesiredMovementThisFrame);
        } 
         

       //Get the static mesh of the chosen Actor
       // UStaticMeshComponent* SM = GetOwner()->FindComponentByClass<UStaticMeshComponent>();
    }
};



