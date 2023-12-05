// Fill out your copyright notice in the Description page of Project Settings.


#include "Mechanisms/SolarPanel.h"

// Default constructor
ASolarPanel::ASolarPanel()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASolarPanel::BeginPlay()
{
	Super::BeginPlay();

	energy = &MyEnergy;
	maxEnergy = &MyMaxEnergy;
	mechanismTitle = &MyMechanismTitle;
	canGiveEnergy = &ThisCanGiveEnergy;
	maxGivePerTick = &ThisMaxGivePerTick;
	powerConsumption = &MyPowerConsumption;

	WidgetClass = WidgetClassRef.TryLoadClass<UUserWidget>(); // Try load class


	if (WidgetClass) // If all OK, we create widget
	{
		Widget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
	}
}

// Called every frame
void ASolarPanel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MyDeltaTime = DeltaTime;
	ASolarPanel::Work();
}

void ASolarPanel::GiveEnergy(TArray<AMechanism*> ConnectedMechanisms)
{
	Super::GiveEnergy(ConnectedMechanisms);
}

bool ASolarPanel::Work()
{
	Super::Work();
	if (MyEnergy+GenerationPerTick < MyMaxEnergy)
	{
		MyEnergy += GenerationPerTick*MyDeltaTime;
		return true;
	}
	else
	{
		MyEnergy = MyMaxEnergy;
		return true;
	}
	return false;
}

void ASolarPanel::ShowWidget()
{
	AMechanism::ShowWidget();
	if (Widget != nullptr) { Widget->AddToViewport(); }
}

void ASolarPanel::HideWidget()
{
	AMechanism::HideWidget();
	if (Widget != nullptr) { Widget->RemoveFromParent(); }
}

TArray<FString> ASolarPanel::GetStatus()
{
	currentStatus.Empty(); // Clear array

	// Main info
	currentStatus.Emplace(MyMechanismTitle); // Title index 0
	currentStatus.Emplace(FString::SanitizeFloat(MyEnergy / MyMaxEnergy)); // Energy percentage i1

	// Energy
	currentStatus.Emplace(FString::SanitizeFloat(MyEnergy)); // Current energy i2
	currentStatus.Emplace(FString::SanitizeFloat(MyMaxEnergy)); // Max energy i3

	currentStatus.Emplace(FString::SanitizeFloat(GenerationPerTick)); // i4
	currentStatus.Emplace(FString::SanitizeFloat(IsEnergyGenerating)); // i5


	return currentStatus;
}