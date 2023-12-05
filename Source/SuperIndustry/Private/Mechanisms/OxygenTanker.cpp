// Fill out your copyright notice in the Description page of Project Settings.


#include "Mechanisms/OxygenTanker.h"

// Default constructor
AOxygenTanker::AOxygenTanker()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AOxygenTanker::BeginPlay()
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

// Called every frame
void AOxygenTanker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MyDeltaTime = DeltaTime;
	IsPaused = false;
	AOxygenTanker::Work(); 
	
	if (OxygenDropStarted)
	{
		DropOxygen(OxygenToDropLeft / 10);
	}
}

int AOxygenTanker::AcceptEnergy(int EnergyToAdd)
{
	return Super::AcceptEnergy(EnergyToAdd);
}

bool AOxygenTanker::Work()
{
	if (Super::Work())
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("wORK")));
		if (mechanismTankVolumecurrent + OxygenPerTick < mechanismTankVolumeMax)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("oxyadd")));
			mechanismTankVolumecurrent += OxygenPerTick*MyDeltaTime;
			return true;
		}
		else
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("oxifool")));
			mechanismTankVolumecurrent = mechanismTankVolumeMax;
			MyEnergy += MyPowerConsumption;
			return false;
		}
	}
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Nonwork")));
	return false;
}

void AOxygenTanker::SetPause(bool pause)
{
	ThisIsPaused = pause;
}


void AOxygenTanker::DropOxygenTo(float OxygenToDrop)
{
	OxygenDropStarted = true;
	OxygenToDropLeft = OxygenToDrop;
}

void AOxygenTanker::DropOxygen(float OxygenToDrop)
{
	if (mechanismTankVolumecurrent - OxygenToDrop > 0)
	{
		mechanismTankVolumecurrent -= OxygenToDrop;
		OxygenToDropLeft -= OxygenToDrop;
	}

	else
	{
		mechanismTankVolumecurrent = 0;
		OxygenToDropLeft = 0;
		OxygenDropStarted = false;
	}
}

void AOxygenTanker::ShowWidget()
{
	AMechanism::ShowWidget();
	if (Widget != nullptr) 
	{ 
		Widget->AddToViewport(); 
		Widget->bIsFocusable = true;
	}
}

void AOxygenTanker::HideWidget()
{
	AMechanism::HideWidget();
	if (Widget != nullptr) { Widget->RemoveFromParent(); }
}

TArray<FString> AOxygenTanker::GetStatus()
{
	currentStatus.Empty(); // Clear array

	// Main info
	currentStatus.Emplace(MyMechanismTitle); // Title index 0
	currentStatus.Emplace(FString::SanitizeFloat(MyEnergy / MyMaxEnergy)); // Energy percentage i1
	
	// Energy 
	currentStatus.Emplace(FString::SanitizeFloat(MyEnergy)); // Current energy i2
	currentStatus.Emplace(FString::SanitizeFloat(MyMaxEnergy)); // Max energy i3

	// Oxygen
	currentStatus.Emplace(FString::SanitizeFloat(mechanismTankVolumeMax)); // Current oxygen i4
	currentStatus.Emplace(FString::SanitizeFloat(mechanismTankVolumecurrent)); // Max oxygen i5

	return currentStatus;
}