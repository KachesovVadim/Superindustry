// Copyright Epic Games, Inc. All Rights Reserved.

#include "SuperIndustryCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Actor.h"

#include "Kismet/GameplayStatics.h"
#include "mechanisms/PocketDrill.h"

//////////////////////////////////////////////////////////////////////////
// ASuperIndustryCharacter

ASuperIndustryCharacter::ASuperIndustryCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rate for input
	TurnRateGamepad = 50.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 250.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void ASuperIndustryCharacter::BeginPlay()
{
	Super::BeginPlay();
	PrimaryActorTick.bCanEverTick = true;

	// Get player controller for disable and enable input
	MyPlayerController = GetWorld()->GetFirstPlayerController();
	LocalPlayer = MyPlayerController->GetLocalPlayer();
}

void ASuperIndustryCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (OxygenRefuelStarted)
	{
		RefuelOxygen(OxygenToAddLeft/10);
	}
}


//////////////////////////////////////////////////////////////////////////
// Input

void ASuperIndustryCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ASuperIndustryCharacter::StartRun);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ASuperIndustryCharacter::StopRun);

	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &ASuperIndustryCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &ASuperIndustryCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &ASuperIndustryCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &ASuperIndustryCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ASuperIndustryCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ASuperIndustryCharacter::TouchStopped);
}

void ASuperIndustryCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void ASuperIndustryCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void ASuperIndustryCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void ASuperIndustryCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void ASuperIndustryCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ASuperIndustryCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}


// --- NEW FUNCTIONS ---

void ASuperIndustryCharacter::StartRun()
{
	GetCharacterMovement()->MaxWalkSpeed = 700.0f;
}

void ASuperIndustryCharacter::StopRun()
{
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;
}

FHitResult ASuperIndustryCharacter::CastInteractionRay(int TraceDistance)
{
	GetController()->GetPlayerViewPoint(StartLoc, StartRot);
	End = StartLoc + (StartRot.Vector() * TraceDistance);
	
	GetWorld()->LineTraceSingleByChannel(Result,
										StartLoc,
										End,
										ECC_Visibility,
										TraceParams);
	
	return Result;
}

int ASuperIndustryCharacter::IsInteractionRayHitMechanism(int TraceDistance)
{
	FHitResult LineTraceResult = CastInteractionRay(TraceDistance); 
	HitActor = LineTraceResult.GetActor();
	if (IsValid(HitActor)) // If actor hitted
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, HitActor->GetClass()->GetName());
		if((HitActor->GetClass())->IsChildOf(AMechanism::StaticClass())) // If actor's parent - mechanism
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("None")));
			HittedMechanism = (AMechanism*)HitActor;
			InteractedMechanismActor = HitActor;
			if (!NeedToShowMechanismInfoHud)
			{
				NeedToShowMechanismInfoHud = true;
				return 1; // Add to viewport
			}
			return 0; // To do nothing
		}
		
		else
		{
			if (NeedToShowMechanismInfoHud)
			{
				NeedToShowMechanismInfoHud = false;
				return -1; // Remove widget
			}
			return 0; // To do nothing
		}
	}
	
	if (NeedToShowMechanismInfoHud) // If the player didnt look at anything
	{
		NeedToShowMechanismInfoHud = false;
		return -1; // Remove widget
	}
	return 0; // To do nothing
}

void ASuperIndustryCharacter::MemorizeMechanism()
{
	InteractedMechanism = HittedMechanism;
}

void ASuperIndustryCharacter::Disable_Input()
{
	MyPlayerController->SetInputMode(FInputModeUIOnly());
	MyPlayerController->PlayerInput->FlushPressedKeys();
	MyPlayerController->bShowMouseCursor = true;
}

void ASuperIndustryCharacter::Enable_Input() 
{
	MyPlayerController->SetInputMode(FInputModeGameOnly());
	MyPlayerController->bShowMouseCursor = false;
}

void ASuperIndustryCharacter::CenterCursor()
{
	Viewport = LocalPlayer->ViewportClient->Viewport; // Get viewport
	
	LocalPlayer->ViewportClient->GetViewportSize(ViewportSize);
	int32 X = ViewportSize.X * 0.5f;
	int32 Y = ViewportSize.Y * 0.5f;
	Viewport->SetMouse(X, Y);
}

void ASuperIndustryCharacter::StartRefuelOxygen(float OxygenToAdd)
{
	OxygenRefuelStarted = true;
	OxygenToAddLeft = OxygenToAdd;
}

void ASuperIndustryCharacter::RefuelOxygen(float OxygenToAdd)
{
	if (OxygenInBallons + OxygenToAdd < MaxOxygen)
	{
		OxygenInBallons += OxygenToAdd;
		OxygenToAddLeft -= OxygenToAdd;
	}

	else
	{
		OxygenToAdd = OxygenToAdd - (MaxOxygen - OxygenInBallons);
		OxygenInBallons = MaxOxygen;
		
		OxygenToAddLeft -= OxygenToAdd;
		
		OxygenRefuelStarted = false;
	}
}