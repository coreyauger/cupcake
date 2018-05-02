// Fill out your copyright notice in the Description page of Project Settings.


#include "SpeedBoostTriggerVolume.h"
#include "../Debug.h"


ASpeedBoostTriggerVolume::ASpeedBoostTriggerVolume(){
     //Register Events
    OnActorBeginOverlap.AddDynamic(this, &ASpeedBoostTriggerVolume::OnOverlapBegin);
    OnActorEndOverlap.AddDynamic(this, &ASpeedBoostTriggerVolume::OnOverlapEnd);
}



