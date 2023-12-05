// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Mechanisms/Mechanism.h"
#include "OxygenTanker.generated.h"

UCLASS()
class SUPERINDUSTRY_API AOxygenTanker : public AMechanism
{
	GENERATED_BODY()

public:
	AOxygenTanker();

protected:
	void BeginPlay() override;

public:
	// Called every frame
	void Tick(float DeltaTime) override;
	float MyDeltaTime;
	
	// Making variables
	float MyEnergy = 0;
	float MyMaxEnergy = 500;
	FString MyMechanismTitle = FString(TEXT("Oxygen tanker"));
	
	bool ThisCanGiveEnergy = false;
	int MyPowerConsumption = 1.5;
	float OxygenPerTick = 0.5;
	UPROPERTY(BlueprintReadWrite) bool ThisIsPaused = false;

	TArray<FString> currentStatus; 

	float mechanismTankVolumeMax = 200; // In liters
	float mechanismTankVolumecurrent = 0;
	bool OxygenDropStarted = false;
	float OxygenToDropLeft = 0;

	// Mechanism widget
	UPROPERTY() // UPROPERTY, because UE delete variables
	FSoftClassPath WidgetClassRef = TEXT("WidgetBlueprint'/Game/Blueprints/HUD/Mechanisms/OxygenTanker/OxygenTankerHUD.OxygenTankerHUD_C'"); // Widget BP class reference in a string
	UPROPERTY()
	UClass* WidgetClass;
	UPROPERTY()
	UUserWidget* Widget; // Widget for adding to viewport
			

	// Making functions

	// Interacting
	int AcceptEnergy(int) override;
	bool Work() override;
	UFUNCTION(BlueprintCallable) void SetPause(bool pause) override;
	
	UFUNCTION(BlueprintCallable) void DropOxygenTo(float OxygenToSet);
	void DropOxygen(float);

	void ShowWidget() override;
	void HideWidget() override;

	TArray<FString> GetStatus() override;
};