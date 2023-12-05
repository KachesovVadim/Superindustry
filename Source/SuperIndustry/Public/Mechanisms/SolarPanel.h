// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Mechanisms/Mechanism.h"
#include "SolarPanel.generated.h"


UCLASS()
class SUPERINDUSTRY_API ASolarPanel : public AMechanism
{
	GENERATED_BODY()

public:
	ASolarPanel();

protected:
	void BeginPlay() override;

public:
	// Called every frame
	void Tick(float DeltaTime) override;
	float MyDeltaTime;

	// Making variables
	float MyEnergy = 0;
	float MyMaxEnergy = 2000;
	
	bool ThisCanGiveEnergy = true;
	int ThisMaxGivePerTick = 2;
	float GenerationPerTick = 11;
	bool IsEnergyGenerating = true;
	int MyPowerConsumption = 0;
	
	FString MyMechanismTitle = FString(TEXT("Solar panel"));
	TArray<FString> currentStatus;

	// Mechanism widget
	UPROPERTY() // UPROPERTY, because UE delete variables
	FSoftClassPath WidgetClassRef = TEXT("WidgetBlueprint'/Game/Blueprints/HUD/Mechanisms/SolarPanel/SolarPanelHUD.SolarPanelHUD_C'"); // Widget BP class reference in a string
	UPROPERTY()
	UClass* WidgetClass;
	UPROPERTY()
	UUserWidget* Widget; // Widget for adding to viewport

	// Making functions

	// Interacting
	UFUNCTION(BlueprintCallable)
	void GiveEnergy(TArray<AMechanism*> ConnectedMechanisms) override;
	bool Work() override;
	
	void ShowWidget() override;
	void HideWidget() override;

	TArray<FString> GetStatus() override;
};