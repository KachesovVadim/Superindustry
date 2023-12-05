// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Mechanisms/Mechanism.h"
#include "PocketDrill.generated.h"



UCLASS()
class SUPERINDUSTRY_API APocketDrill : public AMechanism
{
	GENERATED_BODY()

public:
	APocketDrill();

protected:
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	float MyDeltaTime;

	// Making variables
	float MyEnergy = 100;
	float MyMaxEnergy = 2000;
	UPROPERTY(BlueprintReadOnly) FString MyMechanismTitle = FString(TEXT("Pocket career"));
	
	bool ThisCanGiveEnergy = false;
	int MyPowerConsumption = 6;
	UPROPERTY(BlueprintReadWrite) bool ThisIsPaused = false;

	int32 MinMiningLevel = 1; // Meter
	int32 MaxMiningLevel = 10; // Meters

	TArray<FString> currentStatus;

	// Mechanism widget
	UPROPERTY() // UPROPERTY, because UE delete variables
	FSoftClassPath WidgetClassRef = TEXT("WidgetBlueprint'/Game/Blueprints/HUD/Mechanisms/PocketDrill/PocketDrillHUD.PocketDrillHUD_C'"); // Widget BP class reference in a string
	UPROPERTY()
	UClass* WidgetClass; 
	UPROPERTY()
	UUserWidget* Widget; // Widget for adding to viewport

	// Making functions

	// Interacting
	int AcceptEnergy(int) override;
	UFUNCTION(BlueprintCallable) bool Work(float ConsumptionMultiplier);
	UFUNCTION(BlueprintCallable) void SetPause(bool pause);

	void ShowWidget() override;
	void HideWidget() override;

	TArray<FString> GetStatus() override;
};
