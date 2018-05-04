// Fill out your copyright notice in the Description page of Project Settings.


#include "SpeedBoostTriggerVolume.h"
#include "../Debug.h"
#include "ConstructorHelpers.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "../Pawns/PrimaryPawn.h"


ASpeedBoostTriggerVolume::ASpeedBoostTriggerVolume(){
     //Register Events
    OnActorBeginOverlap.AddDynamic(this, &ASpeedBoostTriggerVolume::OnOverlapBegin);
    OnActorEndOverlap.AddDynamic(this, &ASpeedBoostTriggerVolume::OnOverlapEnd);
    
    SpeedBoostMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpeedBoostMesh"));
    // CA - This is key to having the mesh travel with the trigger volume
    if(SpeedBoostMesh)SpeedBoostMesh->SetupAttachment(RootComponent);    

    // Audio 
    static ConstructorHelpers::FObjectFinder<USoundCue> speedBoostCue(TEXT("/Game/Sounds/speedBoost_Cue"));
    boostAudioCue = speedBoostCue.Object;
    boostAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("SpeedBoostAudioComp"));
    if(!boostAudioComponent){
        UE_LOG(LogTemp, Warning, TEXT("Speed Boost could not create audio component"));
    }else{
        boostAudioComponent->SetupAttachment(RootComponent);    
    }
}


void ASpeedBoostTriggerVolume::PostInitializeComponents(){
	Super::PostInitializeComponents();
	if (boostAudioCue->IsValidLowLevelFast()) {
		boostAudioComponent->SetSound(boostAudioCue);
	}
}

void ASpeedBoostTriggerVolume::BeginPlay()
{
	Super::BeginPlay();
    
	DrawDebugBox(GetWorld(), GetActorLocation(), GetActorScale()*100, FColor::Cyan, true, -1, 0, 5);
	if(SpeedBoostMesh){
        SpeedBoostMesh->SetWorldLocation(GetActorLocation());
    }
}

void ASpeedBoostTriggerVolume::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor)
{
    UE_LOG(LogTemp, Warning, TEXT("Overlap Begin"));
    if (OtherActor && (OtherActor != this)) {
        // print to screen using above defined method when actor enters trigger volume
       
        if(OtherActor->IsA(APrimaryPawn::StaticClass())) {
            print("Overlap Begin");
            printFString("Other Actor = %s", *OtherActor->GetName());
            UE_LOG(LogTemp, Warning, TEXT("Overlap Begin Other Actor = %s"), *OtherActor->GetName() );
            auto PrimaryPawn = static_cast<APrimaryPawn*>(OtherActor);
            PrimaryPawn->AddInputVector( FVector(5000.0, 0.0, 0.0) );
            
            if(boostAudioComponent){
                //float startTime = 9.f;
                //float volume = 1.0f;
                //float fadeTime = 1.0f;
                //boostAudioComponent->FadeIn(fadeTime, volume, startTime);
        
                // Or you can start playing the sound immediately.
                boostAudioComponent->Play();
            }

        // TODO: check if the Other actor is an instance of "game pawn" (we might want enemy ai to respond to boost as well)
        }
        // type cast and apply the force..
        // pawnMovementComponent->AddInputVector(thrust);	
    }
}

void ASpeedBoostTriggerVolume::OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor)
{
    if (OtherActor && (OtherActor != this)) {
        // print to screen using above defined method when actor leaves trigger volume
        //print("Overlap Ended");
        //printFString("%s has left the Trigger Volume", *OtherActor->GetName());
        
    }
}