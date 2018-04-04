// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/MeshComponent.h"
#include "PrimaryPawn.generated.h"

UCLASS()
class CUPCAKE_API APrimaryPawn : public APawn
{
	GENERATED_BODY() 

public:
	// Sets default values for this pawn's properties
	APrimaryPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UCameraComponent* camera = nullptr;
	FVector2D mTouchBegin; 
	FVector2D mTouchMoved; 
	APlayerController *controller = nullptr;
		
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* mesh = nullptr;

	UPROPERTY(EditAnywhere) 
	USpringArmComponent* springArm = nullptr;

	/** Touch Begin 2D Screen corrd */
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Swipe")
	UFUNCTION(BlueprintCallable, Category="Swipe")
	void TouchBegin(const FVector2D &touchBegin);

	/** Touch End in 2d screen space  */
	UFUNCTION(BlueprintCallable, Category="Swipe")
	void TouchEnd(const FVector2D &touchEnd);

	/** Touch Moved in 2d screen space  */
	UFUNCTION(BlueprintCallable, Category="Swipe")
	void TouchMoved(const FVector2D &touchMoved);

	/** Player Single Tap Corrd in 2d screen space */
	UFUNCTION(BlueprintCallable, Category="Swipe")
	void SingleTap(const FVector2D &singleTap);
	
};
