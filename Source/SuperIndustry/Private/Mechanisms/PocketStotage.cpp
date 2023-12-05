// Fill out your copyright notice in the Description page of Project Settings.


#include "Mechanisms/PocketStotage.h"

// Default constructor
APocketStotage::APocketStotage()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APocketStotage::BeginPlay()
{
	Super::BeginPlay();

	energy = &MyEnergy;
	maxEnergy = &MyMaxEnergy;
	mechanismTitle = &MyMechanismTitle;
	canGiveEnergy = &ThisCanGiveEnergy;
	powerConsumption = &MyPowerConsumption;

	WidgetClass = WidgetClassRef.TryLoadClass<UUserWidget>(); // Try load class

	if (WidgetClass) // If all OK, we create widget
	{
		Widget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
	}
}

// Called every frame
void APocketStotage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

int APocketStotage::AcceptEnergy(int EnergyToAdd)
{
	return Super::AcceptEnergy(EnergyToAdd);
}

bool APocketStotage::Work()
{
	if (Super::Work())
	{
		return true;
	}
	return false;
}

void APocketStotage::ShowWidget()
{
	AMechanism::ShowWidget();
	if (Widget != nullptr)
	{
		Widget->AddToViewport();
		Widget->bIsFocusable = true;
	}
}

void APocketStotage::HideWidget()
{
	AMechanism::HideWidget();
	if (Widget != nullptr) { Widget->RemoveFromParent(); }
}

TArray<FString> APocketStotage::GetStatus()
{
	currentStatus.Empty(); // Clear array

	// Main info
	currentStatus.Emplace(MyMechanismTitle); // Title index 0
	currentStatus.Emplace(FString::SanitizeFloat(MyEnergy / MyMaxEnergy)); // Energy percentage i1

	// Energy 
	currentStatus.Emplace(FString::SanitizeFloat(MyEnergy)); // Current energy i2
	currentStatus.Emplace(FString::SanitizeFloat(MyMaxEnergy)); // Max energy i3

	return currentStatus;
}