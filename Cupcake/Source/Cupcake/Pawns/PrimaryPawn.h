// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "CollidingPawnMovementComponent.h"
#include "PrimaryPawn.generated.h"

enum PawnMode{
	Selected,
	Camera,
	Aim
};

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
	virtual void UpdateTouchBegin(const FVector2D &touch);
	virtual void UpdateTouchEnd(const FVector2D &touch);
 
	UCameraComponent* camera = nullptr; 
	FVector2D mTouchBegin;  
	FVector2D mTouchMoved; 
	APlayerController *controller = nullptr;
	UCollidingPawnMovementComponent* pawnMovementComponent = nullptr;
	PawnMode mode = PawnMode::Camera; 

	// when taking a shot to move ball we store the aimVector here.
	FVector mAimVector = FVector(0.0f, 0.0f, 0.0f);

	// PlayerUI
	class UPlayerUI* mPlayerUI = nullptr;
	TSubclassOf<class UUserWidget> PlayerUIClass;
		  
public:	  
	// Called every frame
	virtual void Tick(float DeltaTime) override; 

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual UPawnMovementComponent* GetMovementComponent() const override;
	void AddInputVector(const FVector &thrust);

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* ballMesh = nullptr; 

	UPROPERTY(EditAnywhere) 
	class USpringArmComponent* springArm = nullptr; 

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
