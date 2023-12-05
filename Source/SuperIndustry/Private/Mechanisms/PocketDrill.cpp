// Fill out your copyright notice in the Description page of Project Settings.


#include "Mechanisms/PocketDrill.h"

// Default constructor
APocketDrill::APocketDrill() 
{
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APocketDrill::BeginPlay()
{
	Super::BeginPlay();

	energy = &MyEnergy;
	maxEnergy = &MyMaxEnergy;
	mechanismTitle = &MyMechanismTitle;
	canGiveEnergy = &ThisCanGiveEnergy;
	powerConsumption = &MyPowerConsumption;
	IsPaused = &ThisIsPaused;

	WidgetClass = WidgetClassRef.TryLoadClass<UUserWidget>(); // Try load class

	if (WidgetClass) // If all OK, we create widget
	{
		Widget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
	}
}

TArray<FString> APocketDrill::GetStatus()
{
	currentStatus.Empty(); // Clear array

	// Main info
	currentStatus.Emplace(MyMechanismTitle); // Title index 0
	currentStatus.Emplace(FString::SanitizeFloat(MyEnergy / MyMaxEnergy)); // Energy percentage i1

	// Energy
	currentStatus.Emplace(FString::SanitizeFloat(MyEnergy)); // Current energy i2
	currentStatus.Emplace(FString::SanitizeFloat(MyMaxEnergy)); // Max energy i3

	// Mining levels
	currentStatus.Emplace(FString::FromInt(MinMiningLevel)); // i4
	currentStatus.Emplace(FString::FromInt(MaxMiningLevel)); // i5

	return currentStatus;
}

// Called every frame
void APocketDrill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MyDeltaTime = DeltaTime;
}

int APocketDrill::AcceptEnergy(int EnergyToAdd)
{
	return Super::AcceptEnergy(EnergyToAdd);
}

bool APocketDrill::Work(float ConsumptionMultiplier)
{
	if (ThisIsPaused == false)
	{
		if (*energy - *powerConsumption * MyDeltaTime * ConsumptionMultiplier > 0)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("%f"), *powerConsumption * MyDeltaTime));
			*energy -= *powerConsumption * MyDeltaTime * ConsumptionMultiplier;
			return true;
		}
	}
	return false;
}

void APocketDrill::SetPause(bool pause)
{
	ThisIsPaused = pause;
}

void APocketDrill::ShowWidget()
{
	AMechanism::ShowWidget();
	if (Widget != nullptr)
	{
		Widget->AddToViewport();
		Widget->bIsFocusable = true;
	}
}

void APocketDrill::HideWidget()
{
	AMechanism::HideWidget();
	if (Widget) { Widget->RemoveFromParent(); }
}