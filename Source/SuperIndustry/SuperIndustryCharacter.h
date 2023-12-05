// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Mechanisms/Mechanism.h"

#include "GameFramework/Character.h"
#include "SuperIndustryCharacter.generated.h"

UCLASS(config=Game)
class ASuperIndustryCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	ASuperIndustryCharacter();
	//virtual void Tick(float DeltaTime) override;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input)
	float TurnRateGamepad;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }


	// --- NEW VARIABLES ---

	UPROPERTY(BlueprintReadWrite) float OxygenInBallons = 1;
	UPROPERTY(BlueprintReadOnly) float MaxOxygen = 100; // In liters

	APlayerController* MyPlayerController;
	const ULocalPlayer* LocalPlayer;
	
	FViewport* Viewport;
	FVector2D ViewportSize;

	bool OxygenRefuelStarted = false;
	float OxygenToAddLeft = 0; // The amount of oxygen left to add
	

	// --- NEW FUNCTIONS ---

	void StartRun();
	void StopRun();
	
	UFUNCTION(BlueprintCallable)
	void Disable_Input();
	UFUNCTION(BlueprintCallable)
	void Enable_Input();
	void CenterCursor();
	
	UFUNCTION(BlueprintCallable)
	void StartRefuelOxygen(float OxygenToAdd);
	void RefuelOxygen(float);
	
	// This function trace a line
	FVector StartLoc;
	FRotator StartRot;
	FHitResult Result;
	FCollisionQueryParams TraceParams;
	FVector End;

	UFUNCTION(BlueprintCallable)
	FHitResult CastInteractionRay(int TraceDistance);

	/* This function trace a line and then checks if the ray has passed
	through the mechanism */
	bool NeedToShowMechanismInfoHud = false;
	AActor* HitActor;
	UPROPERTY(BlueprintReadOnly) AMechanism* HittedMechanism;
	UPROPERTY(BlueprintReadOnly) AMechanism* InteractedMechanism; // Last interacted mechanism
	UPROPERTY(BlueprintReadOnly) AActor* InteractedMechanismActor;

	UFUNCTION(BlueprintCallable)
	int IsInteractionRayHitMechanism(int TraceDistance);

	UFUNCTION(BlueprintCallable)
	void MemorizeMechanism();
};
